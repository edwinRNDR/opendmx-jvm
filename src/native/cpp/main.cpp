#include "org_openrndr_opendmx_Native.h"
#include <jni.h>

extern "C" {
#include <ftd2xx.h>
}
#include <cstdio>

FT_HANDLE ftHandle;

JNIEXPORT jboolean JNICALL Java_org_openrndr_opendmx_Native_connect
  (JNIEnv *, jclass, jint) {

    FT_STATUS ftStatus;
    char Buf[64];
    FT_ListDevices(0, Buf, FT_LIST_BY_INDEX| FT_OPEN_BY_DESCRIPTION);
    ftHandle = FT_W32_CreateFile(Buf, GENERIC_READ|GENERIC_WRITE, 0, 0,
    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FT_OPEN_BY_DESCRIPTION, 0);

    if (ftHandle == INVALID_HANDLE_VALUE) {
        return false;
    } else {
        FTDCB ftDCB;
        if (FT_W32_GetCommState(ftHandle, &ftDCB)) {
            ftDCB.BaudRate = 250000;
            ftDCB.Parity = FT_PARITY_NONE;
            ftDCB.StopBits = FT_STOP_BITS_2;
            ftDCB.ByteSize = FT_BITS_8;
            ftDCB.fOutX = false;
            ftDCB.fInX = false;
            ftDCB.fErrorChar = false;
            ftDCB.fBinary = true;
            ftDCB.fRtsControl = false;
            ftDCB.fAbortOnError = false;

            if (!FT_W32_SetCommState(ftHandle, &ftDCB)) {
                printf("failed to set baudrate\n");
            }
        }
        FT_W32_EscapeCommFunction(ftHandle, CLRRTS);
        FT_W32_PurgeComm(ftHandle, FT_PURGE_TX | FT_PURGE_RX);
    }
    return true;
}

JNIEXPORT void JNICALL Java_org_openrndr_opendmx_Native_send
  (JNIEnv *env, jclass, jbyteArray arr) {
    jint elementCount = env->GetArrayLength(arr);

    if (elementCount == 512) {
        boolean isCopy;
        jbyte *b = env->GetByteArrayElements(arr, &isCopy );

        unsigned char startCode = 0;

        FT_W32_SetCommBreak(ftHandle);
        FT_W32_ClearCommBreak(ftHandle);

        ULONG bytesWritten = 0;
        FT_W32_WriteFile(ftHandle, &startCode, 1, &bytesWritten, NULL);
        FT_W32_WriteFile(ftHandle, b, 512, &bytesWritten, NULL);

        env->ReleaseByteArrayElements(arr, b, JNI_ABORT);
    }
}