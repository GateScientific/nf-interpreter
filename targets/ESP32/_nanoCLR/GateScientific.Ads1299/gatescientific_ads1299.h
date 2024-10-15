// Copyright (c) Gate Scientific Inc.


#ifndef GATESCIENTIFIC_ADS1299_H
#define GATESCIENTIFIC_ADS1299_H

#include <nanoCLR_Interop.h>
#include <nanoCLR_Runtime.h>
#include <nanoPackStruct.h>
#include <corlib_native.h>
#include <sys_dev_spi_native.h>

typedef enum __nfpack OutputDataRate
{
    OutputDataRate__16kSamples = 0,
    OutputDataRate__8kSamples = 1,
    OutputDataRate__4kSamples = 2,
    OutputDataRate__2kSamples = 3,
    OutputDataRate__1kSamples = 4,
    OutputDataRate__500Samples = 5,
    OutputDataRate__250Samples = 6,
} OutputDataRate;

typedef enum __nfpack Register
{
    Register_Id = 0,
    Register_Config1 = 1,
} Register;

struct Library_gatescientific_ads1299_GateScientific_Ads1299_Ads1299
{
    static const int FIELD_STATIC___sampleRates = 0;

    static const int FIELD__SpiCsPinNumber = 1;
    static const int FIELD__SpiCsPin = 2;
    static const int FIELD__StartPin = 3;
    static const int FIELD__AdcOn = 4;
    static const int FIELD___disposedValue = 5;
    static const int FIELD___spiDevice = 6;
    static const int FIELD___outputDataRate = 7;

    NANOCLR_NATIVE_DECLARE(NativeInit___VOID);
    NANOCLR_NATIVE_DECLARE(NativeDeInit___VOID);
    NANOCLR_NATIVE_DECLARE(NativeRecordEmg___SZARRAY_U1__SZARRAY_U1);

    //--//
};

extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_GateScientific_Ads1299;

#endif // GATESCIENTIFIC_ADS1299_H
