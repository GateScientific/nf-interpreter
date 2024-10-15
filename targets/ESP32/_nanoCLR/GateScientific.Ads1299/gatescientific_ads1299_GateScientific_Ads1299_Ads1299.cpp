// Copyright (c) Gate Scientific Inc.

#include "gatescientific_ads1299.h"

typedef Library_sys_dev_spi_native_System_Device_Spi_SpiConnectionSettings SpiConnectionSettings;
typedef Library_sys_dev_spi_native_System_Device_Spi_SpiDevice SpiDevice;

static int32_t spiDeviceHandle;


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

    NANOCLR_SET_AND_LEAVE(stack.NotImplementedStub());

    NANOCLR_NOCLEANUP();
}

HRESULT Library_gatescientific_ads1299_GateScientific_Ads1299_Ads1299::
    NativeRecordEmg___VOID__SZARRAY_U1__SystemTimeSpan(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();

    // get a pointer to the managed object instance and check that it's not NULL
    CLR_RT_HeapBlock *pThis = stack.This();
    FAULT_ON_NULL(pThis);

    // release the DRDY pin
    CPU_GPIO_ReservePin(dataReadyPin, false);

    NANOCLR_NOCLEANUP();
}
