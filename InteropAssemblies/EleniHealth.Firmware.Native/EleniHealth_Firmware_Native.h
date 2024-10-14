//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------

#ifndef ELENIHEALTH_FIRMWARE_NATIVE_H
#define ELENIHEALTH_FIRMWARE_NATIVE_H

#include <nanoCLR_Interop.h>
#include <nanoCLR_Runtime.h>
#include <nanoPackStruct.h>

typedef enum __nfpack RMTHelper_TxChannel
{
    RMTHelper_TxChannel_CHANNEL_0 = 0,
    RMTHelper_TxChannel_CHANNEL_1 = 1,
    RMTHelper_TxChannel_CHANNEL_2 = 2,
    RMTHelper_TxChannel_CHANNEL_3 = 3,
} RMTHelper_TxChannel;

struct Library_EleniHealth_Firmware_Native_EleniHealth_Firmware_Native_RMTHelper
{
    NANOCLR_NATIVE_DECLARE(NativeGetRegister___STATIC__U4__U4);
    NANOCLR_NATIVE_DECLARE(NativeSetRegisterOrOp___STATIC__VOID__U4__U4);
    NANOCLR_NATIVE_DECLARE(NativeSetRegisterAndOp___STATIC__VOID__U4__U4);

    //--//
};

extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_EleniHealth_Firmware_Native;

#endif // ELENIHEALTH_FIRMWARE_NATIVE_H
