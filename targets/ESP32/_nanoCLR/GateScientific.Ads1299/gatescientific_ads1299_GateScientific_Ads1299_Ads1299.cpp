// Copyright (c) Gate Scientific Inc.

#include "gatescientific_ads1299.h"

#define BYTES_PER_READING     3
#define STATUS_BYTES_COUNT    3
#define CHANNEL_1_DATA_OFFSET 3
#define NUMBER_OF_CHANNELS    4

typedef Library_sys_dev_spi_native_System_Device_Spi_SpiConnectionSettings SpiConnectionSettings;
typedef Library_sys_dev_spi_native_System_Device_Spi_SpiDevice SpiDevice;

static int32_t spiDeviceHandle;
static GPIO_PIN csPin;
static GPIO_PIN dataReadyPin;
static NF_GS_EMGDATA emgData;
static SPI_WRITE_READ_SETTINGS spiWrSettings = {
    .fullDuplex = true,
    .readOffset = 0,
    .Bits16ReadWrite = false,
    .callback = NULL,
    .DeviceChipSelect = -1,
    .ChipSelectActiveState = false};

// buffer to hold:
// - 3 bytes for the status data
// - 3 bytes for each channel data
static uint8_t readBuffer[STATUS_BYTES_COUNT + NUMBER_OF_CHANNELS * BYTES_PER_READING];

void DataReadyHandler(GPIO_PIN pinNumber, bool pinState, void *pArg)
{
    (void)pArg;
    (void)pinNumber;
    (void)pinState;

    if(emgData.ReadingsToComplete > 0)
    { 
        // read the data
        nanoSPI_Write_Read(spiDeviceHandle, spiWrSettings, NULL, 0, &readBuffer[0], sizeof(readBuffer));

        // copy to the managed buffer:
        // - dropping the status data
        // - copying only channel 1 data
        memcpy(emgData.Buffer, &readBuffer[CHANNEL_1_DATA_OFFSET], BYTES_PER_READING);

        // decrement the number of readings to complete
        emgData.ReadingsToComplete--;

        // move pointer to next position
        emgData.Buffer += BYTES_PER_READING;

        // check if all readings have been completed
        if (emgData.ReadingsToComplete == 0)
        {
            // all readings have been completed

            // no matter the result, send the command to Stop Read Data Continuously mode
            uint8_t workBuffer = 0b00010001;

            nanoSPI_Write_Read(spiDeviceHandle, spiWrSettings, (uint8_t *)&workBuffer, 1, NULL, 0);

            // de-assert CS
            CPU_GPIO_SetPinState(csPin, GpioPinValue_High);

            /////////////////////////////////////////////
            // HIJACKING the FLAG_RADIO for this event //
            /////////////////////////////////////////////
            Events_Set(SYSTEM_EVENT_FLAG_RADIO);
        }
    }
}

HRESULT Library_gatescientific_ads1299_GateScientific_Ads1299_Ads1299::NativeInit___VOID(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();

    CLR_RT_HeapBlock *spiDevice;

    // get a pointer to the managed object instance and check that it's not NULL
    CLR_RT_HeapBlock *pThis = stack.This();
    FAULT_ON_NULL(pThis);

    // get a pointer to the SPI device
    spiDevice = pThis[FIELD___spiDevice].Dereference();

    // get the SPI device handle
    spiDeviceHandle = spiDevice[SpiDevice::FIELD___deviceId].NumericByRef().s4;

    // get the CS pin number
    csPin = (GPIO_PIN)pThis[FIELD__SpiCsPinNumber].NumericByRef().s4;

    // get the DRDY pin
    dataReadyPin = (GPIO_PIN)pThis[FIELD__DataReadyPinNumber].NumericByRef().s4;

    // setup the DRDY pin
    if (!CPU_GPIO_ReservePin(dataReadyPin, true))
    {
        NANOCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);
    }

    CPU_GPIO_EnableInputPin(dataReadyPin, 0, DataReadyHandler, NULL, GPIO_INT_LEVEL_LOW, PinMode_Input);

    NANOCLR_NOCLEANUP();
}

HRESULT Library_gatescientific_ads1299_GateScientific_Ads1299_Ads1299::NativeDeInit___VOID(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();

    // get a pointer to the managed object instance and check that it's not NULL
    CLR_RT_HeapBlock *pThis = stack.This();
    FAULT_ON_NULL(pThis);

    // release the DRDY pin
    CPU_GPIO_DisablePin(dataReadyPin, PinMode_Input, 0);

    NANOCLR_NOCLEANUP();
}

HRESULT Library_gatescientific_ads1299_GateScientific_Ads1299_Ads1299::
    NativeRecordEmg___VOID__SZARRAY_U1__I4__SystemTimeSpan(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();

    uint8_t workBuffer = 0;
    int64_t *timeoutTicks;
    bool eventResult = true;

    CLR_RT_HeapBlock_Array *buffer;
    CLR_RT_HeapBlock hbTimeout;

    // get a pointer to the managed object instance and check that it's not NULL
    CLR_RT_HeapBlock *pThis = stack.This();
    FAULT_ON_NULL(pThis);

    // check disposed state
    if (pThis[FIELD___disposedValue].NumericByRef().u1 != 0)
    {
        NANOCLR_SET_AND_LEAVE(CLR_E_OBJECT_DISPOSED);
    }

    // get the buffer
    buffer = stack.Arg1().DereferenceArray();

    // setup timeout
    // NANOCLR_CHECK_HRESULT(stack.SetupTimeoutFromTimeSpan(stack.Arg3(), timeoutTicks));

    // // add an extra 20s to the timeout, to allow for the EMG data to be read
    // *timeoutTicks += 20000000;

    // this is longer than the thread time quantum
    hbTimeout.SetInteger((CLR_INT64)-1);

    // if m_customState == 0 then push timeout on to eval stack[0] then move to m_customState = 1
    NANOCLR_CHECK_HRESULT(stack.SetupTimeoutFromTicks(hbTimeout, timeoutTicks));

    if (stack.m_customState == 1)
    {
        // fill in the EMG data structure
        emgData.Buffer = buffer->GetFirstElement();
        emgData.ReadingsToComplete = stack.Arg2().NumericByRef().s4;

        // send Read Data Continuous command
        // RDATAC
        workBuffer = 0b00010000;

        // assert CS
        CPU_GPIO_SetPinState(csPin, GpioPinValue_Low);

        nanoSPI_Write_Read(spiDeviceHandle, spiWrSettings, (uint8_t *)&workBuffer, 1, NULL, 0);

        // bump custom state
        stack.m_customState = 2;
    }

    while (eventResult)
    {
        // non-blocking wait allowing other threads to run while we wait for the read operation to complete

        /////////////////////////////////////////////
        // HIJACKING the FLAG_RADIO for this event //
        /////////////////////////////////////////////
        NANOCLR_CHECK_HRESULT(
            g_CLR_RT_ExecutionEngine.WaitEvents(stack.m_owningThread, *timeoutTicks, Event_Radio, eventResult));

        if (eventResult)
        {
            // done here
            break;
        }
        else
        {
            // timeout has expired
            // return exception
            NANOCLR_SET_AND_LEAVE(CLR_E_TIMEOUT);
        }
    }

    // pop timeout heap block from stack
    stack.PopValue();

    NANOCLR_NOCLEANUP();
}
