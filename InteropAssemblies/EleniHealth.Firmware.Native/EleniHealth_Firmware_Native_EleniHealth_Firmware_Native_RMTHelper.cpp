//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------

#include "EleniHealth_Firmware_Native.h"
#include "EleniHealth_Firmware_Native_EleniHealth_Firmware_Native_RMTHelper.h"

using namespace EleniHealth_Firmware_Native::EleniHealth_Firmware_Native;


unsigned int RMTHelper::NativeGetRegister(unsigned int param0, HRESULT& hr)
{
    (void)hr;
    unsigned int retValue = *((unsigned int*)(param0));

    return retValue;
}

void RMTHelper::NativeSetRegisterOrOp(unsigned int param0, unsigned int param1, HRESULT& hr)
{
    (void)hr;

    *((uint32_t*)(param0)) |= param1;
}

void RMTHelper::NativeSetRegisterAndOp(unsigned int param0, unsigned int param1, HRESULT& hr)
{
    (void)hr;

    *((uint32_t*)(param0)) &= param1;
}
