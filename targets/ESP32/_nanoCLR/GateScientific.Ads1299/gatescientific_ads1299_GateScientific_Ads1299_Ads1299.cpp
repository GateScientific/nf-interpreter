// Copyright (c) Gate Scientific Inc.

#include "gatescientific_ads1299.h"

typedef Library_sys_dev_spi_native_System_Device_Spi_SpiConnectionSettings SpiConnectionSettings;
typedef Library_sys_dev_spi_native_System_Device_Spi_SpiDevice SpiDevice;

static int32_t spiDeviceHandle;


HRESULT Library_gatescientific_ads1299_GateScientific_Ads1299_Ads1299::NativeInit___VOID(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();

    //uint32_t handle = -1;

    CLR_RT_HeapBlock *spiDevice;

    // get a pointer to the managed object instance and check that it's not NULL
    CLR_RT_HeapBlock *pThis = stack.This();
    FAULT_ON_NULL(pThis);

    // get a pointer to the SPI device
    spiDevice = pThis[FIELD___spiDevice].Dereference();

    // get the SPI device handle
    spiDeviceHandle = spiDevice[SpiDevice::FIELD___deviceId].NumericByRef().s4;


    NANOCLR_NOCLEANUP();
}

HRESULT Library_gatescientific_ads1299_GateScientific_Ads1299_Ads1299::NativeDeInit___VOID(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();

    NANOCLR_SET_AND_LEAVE(stack.NotImplementedStub());

    NANOCLR_NOCLEANUP();
}

HRESULT Library_gatescientific_ads1299_GateScientific_Ads1299_Ads1299::NativeRecordEmg___SZARRAY_U1__SZARRAY_U1(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();

    NANOCLR_SET_AND_LEAVE(stack.NotImplementedStub());

    NANOCLR_NOCLEANUP();
}
