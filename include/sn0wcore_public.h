/*
 * sn0wcore "Public" API.
 */
 
//
// !!! THIS IS JUST A PREVIEW OF WHAT IS TO COME !!!
//

#ifndef _sn0wcore_public_h
#define _sn0wcore_public_h

#include <stdio.h>
#include "abirev.h"

#define API_MAJOR_LATEST    1
#define API_MINOR_LATEST    0

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(WIN32)

#include <stdint.h>
#ifndef VOID
#define VOID void
#endif
typedef uint8_t BYTE;
typedef uint8_t UCHAR;
typedef uint8_t *PUCHAR;
typedef uint16_t USHORT;
typedef uint32_t ULONG;
typedef void *LPVOID;
typedef int16_t BOOL;
typedef uint32_t *PULONG;
typedef const void *LPCVOID;
typedef uint32_t DWORD;
typedef uint32_t *PDWORD;
typedef uint16_t WORD;
typedef int32_t LONG;
typedef int32_t RESPONSECODE;
typedef const char *LPCSTR;
typedef const BYTE *LPCBYTE;
typedef BYTE *LPBYTE;
typedef DWORD *LPDWORD;
typedef char *LPSTR;
typedef char *LPTSTR;
typedef const char *LPCTSTR;
typedef char *LPCWSTR;
#else
#include <windows.h>
#endif

//
// API decleration
//

#define OPENSN0W_CORE_CLASS_MAGIC       0x574F4E53

#ifdef _MSC_VER
#define SN0WAPI     __cdecl
#else
#define SN0WAPI
#endif

#define SN0W_PRIVATE_API SN0WAPI

#ifndef __in
#define __in
#endif
#ifndef __out
#define __out
#endif
#ifndef __inout
#define __inout
#endif
#ifndef __inopt
#define __inopt
#endif
#ifndef __outopt
#define __outopt
#endif
#ifndef __optional
#define __optional
#endif

typedef enum {
    STATUS_SUCCESS                   = 0x00000000,
    STATUS_FAILURE                   = 0x80000001,
    STATUS_USB_COMMUNICATION_ERROR   = 0x80000002,
    STATUS_FAILURE_DURING_DOWNLOAD   = 0x80000003,
    STATUS_EXPLOIT_FAILURE           = 0x80000004,
    STATUS_NO_DEVICE                 = 0x80000005,
    STATUS_MEMORY_ALLOCATION_FAILURE = 0x80000006,
    STATUS_INVALID_PARAMETERS        = 0x80000007
} SN0W_RETURN;

#ifndef SUCCEEDED
#define SUCCEEDED(Status)               ((long)(Status) >= 0)
#endif
#ifndef FAILED
#define FAILED(Status)                  ((long)(Status) < 0)
#endif
#ifndef IS_ERROR
#define IS_ERROR(Status)                ((unsigned long)(Status) >> 31 == 1)
#endif

typedef
SN0W_RETURN
SN0WAPI
(*PGET_LAST_ERROR_ROUTINE)(
    __in LPVOID Self
    );
    
typedef
ULONG
SN0WAPI
(*PGET_DEVICE_CHIP_ID)(
    __in LPVOID Self
    );
    
typedef
LPSTR
SN0WAPI
(*PGET_DEVICE_SERIAL_NUMBER)(
    __in LPVOID Self
    );

typedef
LPCSTR
SN0WAPI
(*PGET_DEVICE_CANONICAL_NAME)(
    __in LPVOID Self
    );
    
typedef
SN0W_RETURN
SN0WAPI
(*PLOAD_CONFIGURATION_FILE)(
    __in LPVOID Self,
    __in LPSTR Path
    );
    
typedef
SN0W_RETURN
SN0WAPI
(*PFIX_RECOVERY_MODE)(
    __in LPVOID Self
    );
    
typedef
SN0W_RETURN
SN0WAPI
(*PJAILBREAK)(
    __in LPVOID Self
    );
    
typedef
SN0W_RETURN
SN0WAPI
(*PSEND_FILE_TO_DEVICE)(
    __in LPVOID Self,
    __in LPSTR Path
    );
    
typedef
SN0W_RETURN
SN0WAPI
(*PSEND_COMMAND_TO_DEVICE)(
    __in LPVOID Self,
    __in LPSTR Command
    );
    
typedef
SN0W_RETURN
SN0WAPI
(*PSET_KERNEL_PATH)(
    __in LPVOID Self,
    __in LPSTR Path
    );
    
typedef
SN0W_RETURN
SN0WAPI
(*PSET_BOOT_LOGO_PATH)(
    __in LPVOID Self,
    __in LPSTR Path
    );
    
typedef
SN0W_RETURN
SN0WAPI
(*PSET_BUNDLE_PATH)(
    __in LPVOID Self,
    __in LPSTR Path
    );
    
typedef
SN0W_RETURN
SN0WAPI
(*PSET_RAMDISK_PATH)(
    __in LPVOID Self,
    __in LPSTR Path
    );
    
typedef
SN0W_RETURN
SN0WAPI
(*PENABLE_CUSTOM_DFU)(
    __in LPVOID Self
    );

//
// Class Structure
//

typedef struct _OPENSN0W_CORE_CLASS {
    ULONG Magic;
    ULONG AbiRevision;
    USHORT MajorVersion;
    USHORT MinorVersion;
    LPSTR ConfigurationFilePath;
    LPSTR BootArgs;
    LPVOID Reserved;
    LPVOID Reserved0;
    LPVOID Reserved1;
    LPVOID Reserved2;
    PGET_LAST_ERROR_ROUTINE GetLastError;
    PGET_DEVICE_CHIP_ID GetDeviceChipId;
    PGET_DEVICE_SERIAL_NUMBER GetDeviceSerialNumber;
    PGET_DEVICE_CANONICAL_NAME GetDeviceCanonicalName;
    PSEND_COMMAND_TO_DEVICE SendCommandToDevice;
    PSEND_FILE_TO_DEVICE SendFileToDevice;
    PLOAD_CONFIGURATION_FILE LoadConfigurationFile;
    PFIX_RECOVERY_MODE FixRecoveryMode;
    PSET_BOOT_LOGO_PATH SetBootLogoPath;
    PSET_BUNDLE_PATH SetBundlePath;
    PSET_KERNEL_PATH SetKernelPath;
    PENABLE_CUSTOM_DFU EnableDfuPwn;
    PJAILBREAK StartJailbreak;
} OPENSN0W_CORE_CLASS, *POPENSN0W_CORE_CLASS;

//
// Core Api
//
    
POPENSN0W_CORE_CLASS
SN0WAPI
SnInitializeCore(
    __in USHORT RequestedMajorVersion,
    __in USHORT RequestedMinorVersion
    );
    
SN0W_RETURN
SN0WAPI
SnDeinitializeCore(
    __in LPVOID Self
    );

#ifdef __cplusplus
}
#endif

#endif