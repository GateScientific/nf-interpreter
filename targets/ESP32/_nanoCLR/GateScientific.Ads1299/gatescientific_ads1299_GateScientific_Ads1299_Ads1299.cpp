// Copyright (c) Gate Scientific Inc.

#include "gatescientific_ads1299.h"

#define BYTES_PER_READING     3
#define STATUS_BYTES_COUNT    3
#define CHANNEL_1_DATA_OFFSET 3
#define NUMBER_OF_CHANNELS    4
#define READ_BUFFER_SIZE      (STATUS_BYTES_COUNT + NUMBER_OF_CHANNELS * BYTES_PER_READING)

typedef Library_sys_dev_spi_native_System_Device_Spi_SpiConnectionSettings SpiConnectionSettings;
typedef Library_sys_dev_spi_native_System_Device_Spi_SpiDevice SpiDevice;

static int32_t deviceHandle;
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
static uint8_t readBuffer[READ_BUFFER_SIZE];

void DataReadyHandler(void *arg)
{
    (void)arg;

    NATIVE_INTERRUPT_START

    if (emgData.ReadingsToComplete > 0)
    {
        // read the data
        // Set up SPI Transaction
        spi_transaction_t pTrans;

        // use full duplex unless no read data or bus configuration is half duplex
        pTrans.flags = 0;
        pTrans.cmd = 0;
        pTrans.addr = 0;
        // !! length is total bits!!
        pTrans.length = READ_BUFFER_SIZE * 8;
        pTrans.rxlength = 0;
        pTrans.user = NULL;
        pTrans.tx_buffer = NULL;
        pTrans.rx_buffer = &readBuffer[0];

        spi_device_polling_transmit((spi_device_handle_t)spiDeviceHandle, &pTrans);

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

            // reuse the SPI transaction
            pTrans.tx_buffer = &workBuffer;
            pTrans.length = 1;

            spi_device_polling_transmit((spi_device_handle_t)spiDeviceHandle, &pTrans);

            // de-assert CS
            CPU_GPIO_SetPinState(csPin, GpioPinValue_High);

            // disable the interrupt on the DRDY pin
            gpio_intr_disable((gpio_num_t)dataReadyPin);

            /////////////////////////////////////////////
            // HIJACKING the FLAG_RADIO for this event //
            /////////////////////////////////////////////
            Events_Set(SYSTEM_EVENT_FLAG_RADIO);
        }
    }

    NATIVE_INTERRUPT_END
}

static HRESULT SetupDrdyPin()
{
    NANOCLR_HEADER();

    esp_err_t ret;

    if (!CPU_GPIO_ReservePin(dataReadyPin, true))
    {
        NANOCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);
    }

    gpio_config_t GPIOConfig;

    GPIOConfig.pin_bit_mask = (1ULL << dataReadyPin);
    GPIOConfig.mode = GPIO_MODE_INPUT;
    GPIOConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    GPIOConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    // initialize with interrupt disabled
    GPIOConfig.intr_type = GPIO_INTR_DISABLE;

    gpio_config(&GPIOConfig);

    // setup the interrupt handler
    ret = gpio_isr_handler_add((gpio_num_t)dataReadyPin, DataReadyHandler, NULL);
    if (ret != ESP_OK)
    {
        NANOCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);
    }

    NANOCLR_NOCLEANUP();
}

static void FreeDrdyPin()
{
    // reset the GPIO
    gpio_reset_pin((gpio_num_t)dataReadyPin);

    // free the GPIO pin
    CPU_GPIO_ReservePin(dataReadyPin, false);
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
    deviceHandle = spiDevice[SpiDevice::FIELD___deviceId].NumericByRef().s4;
    spiDeviceHandle = CPU_SPI_GetSpiHandle(deviceHandle);

    // get the CS pin number
    csPin = (GPIO_PIN)pThis[FIELD__SpiCsPinNumber].NumericByRef().s4;

    // get the DRDY pin
    dataReadyPin = (GPIO_PIN)pThis[FIELD__DataReadyPinNumber].NumericByRef().s4;

    // setup the DRDY pin
    NANOCLR_CHECK_HRESULT(SetupDrdyPin());

    NANOCLR_NOCLEANUP();
}

HRESULT Library_gatescientific_ads1299_GateScientific_Ads1299_Ads1299::NativeDeInit___VOID(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();

    // get a pointer to the managed object instance and check that it's not NULL
    CLR_RT_HeapBlock *pThis = stack.This();
    FAULT_ON_NULL(pThis);

    // release the DRDY pin
    FreeDrdyPin();

    NANOCLR_NOCLEANUP();
}

HRESULT Library_gatescientific_ads1299_GateScientific_Ads1299_Ads1299::
    NativeRecordEmg___VOID__SZARRAY_U1__I4__SystemTimeSpan(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();

    uint8_t workBuffer = 0;
    int64_t *timeoutTicks;
    bool eventResult = true;
    esp_err_t ret;

    CLR_RT_HeapBlock_Array *buffer = NULL;

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

    // pin it, so GC doesn't move it because we're using it directly
    buffer->Pin();

    // setup timeout
    NANOCLR_CHECK_HRESULT(stack.SetupTimeoutFromTimeSpan(stack.Arg3(), timeoutTicks));

    if (stack.m_customState == 1)
    {
        // set interrupt type on DRDY pin
        ret = gpio_set_intr_type((gpio_num_t)dataReadyPin, GPIO_INTR_LOW_LEVEL);
        if (ret != ESP_OK)
        {
            NANOCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);
        }

        // enable the interrupt on the DRDY pin
        gpio_intr_enable((gpio_num_t)dataReadyPin);

        // fill in the EMG data structure
        emgData.Buffer = buffer->GetFirstElement();
        emgData.ReadingsToComplete = stack.Arg2().NumericByRef().s4;

        // send Read Data Continuous command
        // RDATAC
        workBuffer = 0b00010000;

        // assert CS
        CPU_GPIO_SetPinState(csPin, GpioPinValue_Low);

        nanoSPI_Write_Read(deviceHandle, spiWrSettings, (uint8_t *)&workBuffer, 1, NULL, 0);

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
            break;
        }
    }

    // pop timeout heap block from stack
    stack.PopValue();

    NANOCLR_CLEANUP();

    // we need to clean up if this is not rescheduled
    if (hr != CLR_E_THREAD_WAITING)
    {
        // unpin the buffer
        buffer->Unpin();

        // disable the interrupt on the DRDY pin
        gpio_intr_disable((gpio_num_t)dataReadyPin);
    }

    NANOCLR_CLEANUP_END();
}
