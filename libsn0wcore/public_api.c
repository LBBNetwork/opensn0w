/*
 * opensn0w "Public" API - The API you should use?
 */
 
#include "core.h"
#include "sn0wcore_public.h"
#include <assert.h>

#undef NDEBUG

extern config_file_t *config;
extern char *kernelcache;
extern char *bootlogo;
extern char *url;
extern char *plist;
extern char *ramdisk;
extern int iboot;
extern int dry_run;
extern int gp_payload;
extern int nologo;
extern int pwndfu;
extern int pwnrecovery;
extern int autoboot;
extern int download;
extern int use_shatter;
extern int do_jailbreak;
extern int dump_bootrom;
extern volatile int jailbreaking;

ULONG CurrentErrorStatus = 0;

#define __self_integrity_check(Self) \
    assert(Self != NULL); \
    assert(Self->Magic == OPENSN0W_CORE_CLASS_MAGIC); \
    assert(Self->AbiRevision == SN0W_ABIREV); \
    assert(Self->GetLastError != NULL); \
    assert(Self->GetDeviceChipId != NULL); \
    assert(Self->GetDeviceSerialNumber != NULL); \
    assert(Self->GetDeviceCanonicalName != NULL); \
    assert(Self->SendCommandToDevice != NULL); \
    assert(Self->SendFileToDevice != NULL); \
    assert(Self->LoadConfigurationFile != NULL); \
    assert(Self->FixRecoveryMode != NULL); \
    assert(Self->SetBootLogoPath != NULL); \
    assert(Self->SetBundlePath != NULL); \
    assert(Self->SetKernelPath != NULL); \
    assert(Self->EnableDfuPwn != NULL); \
    assert(Self->StartJailbreak != NULL);
    

VOID
SN0W_PRIVATE_API
SnSetLastError(
    __in ULONG Error
    )
{
    CurrentErrorStatus = Error;
}

//
// Really Public API
//

SN0W_RETURN
SN0WAPI
SnGetLastError(
    __in LPVOID Self
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    
    return CurrentErrorStatus;
}

ULONG
SN0WAPI
SnGetDeviceChipId(
    __in LPVOID Self
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    
    if(device)
        return device->chip_id;
    else
        return 0;
}

LPSTR
SN0WAPI
SnGetDeviceSerialNumber(
    __in LPVOID Self
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    
    if(client)
        return client->serial;
    else
        return NULL;
}

LPCSTR
SN0WAPI
SnGetDeviceCanonicalName(
    __in LPVOID Self
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    
    if(device)
        return device->colloquial_name;
    else
        return NULL;
}

SN0W_RETURN
SN0WAPI
SnLoadConfigurationFile(
    __in LPVOID Self,
    __in LPSTR Path
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    
    if(!Path) {
        SnSetLastError(STATUS_INVALID_PARAMETERS);
        return STATUS_INVALID_PARAMETERS;
    }
    
    config = config_file_load(Path);
    if(!config) {
        SnSetLastError(STATUS_FAILURE);
        return STATUS_FAILURE;
    }
    config_check(config);

    loadmodules(config->entries);
    
    SnSetLastError(STATUS_SUCCESS);
    return STATUS_SUCCESS;
}

SN0W_RETURN
SN0WAPI
SnFixRecoveryMode(
    __in LPVOID Self
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    
    autoboot = 1;

    SnSetLastError(STATUS_SUCCESS);
    return STATUS_SUCCESS;
}

SN0W_RETURN
SN0WAPI
SnJailbreak(
    __in LPVOID Self
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    
    jailbreak();
    
    SnSetLastError(STATUS_SUCCESS);
    return STATUS_SUCCESS;
}

SN0W_RETURN
SN0WAPI
SnSendFileToDevice(
    __in LPVOID Self,
    __in LPSTR Path
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    
    if(!Path) {
        SnSetLastError(STATUS_INVALID_PARAMETERS);
        return STATUS_INVALID_PARAMETERS;
    }
    
    if(!file_exists(Path)) {
        SnSetLastError(STATUS_FAILURE);
        return STATUS_FAILURE;
    }
        
    send_file(Path);
    
    SnSetLastError(STATUS_SUCCESS);
    return STATUS_SUCCESS;
}

SN0W_RETURN
SN0WAPI
SnSendCommandToDevice(
    __in LPVOID Self,
    __in LPSTR Command
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    
    if(!Command) {
        SnSetLastError(STATUS_INVALID_PARAMETERS);
        return STATUS_INVALID_PARAMETERS;
    }
        
    send_file(Command);
    
    SnSetLastError(STATUS_SUCCESS);
    return STATUS_SUCCESS;
}

SN0W_RETURN
SN0WAPI
SnSetBootLogoPath(
    __in LPVOID Self,
    __in LPSTR Path
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    
    if(!Path) {
        SnSetLastError(STATUS_INVALID_PARAMETERS);
        return STATUS_INVALID_PARAMETERS;
    }
    
    if(!file_exists(Path)) {
        SnSetLastError(STATUS_FAILURE);
        return STATUS_FAILURE;
    }
        
    bootlogo = Path;
    
    SnSetLastError(STATUS_SUCCESS);
    return STATUS_SUCCESS;
}

SN0W_RETURN
SN0WAPI
SnSetBundlePath(
    __in LPVOID Self,
    __in LPSTR Path
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    
    if(!Path) {
        SnSetLastError(STATUS_INVALID_PARAMETERS);
        return STATUS_INVALID_PARAMETERS;
    }
    
    if(!file_exists(Path)) {
        SnSetLastError(STATUS_FAILURE);
        return STATUS_FAILURE;
    }
        
    plist = Path;
    
    SnSetLastError(STATUS_SUCCESS);
    return STATUS_SUCCESS;
}

SN0W_RETURN
SN0WAPI
SnSetKernelPath(
    __in LPVOID Self,
    __in LPSTR Path
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    
    if(!Path) {
        SnSetLastError(STATUS_INVALID_PARAMETERS);
        return STATUS_INVALID_PARAMETERS;
    }
    
    if(!file_exists(Path)) {
        SnSetLastError(STATUS_FAILURE);
        return STATUS_FAILURE;
    }
        
    kernelcache = Path;
    
    SnSetLastError(STATUS_SUCCESS);
    return STATUS_SUCCESS;
}

SN0W_RETURN
SN0WAPI
SnSetRamdiskPath(
    __in LPVOID Self,
    __in LPSTR Path
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    
    if(!Path) {
        SnSetLastError(STATUS_INVALID_PARAMETERS);
        return STATUS_INVALID_PARAMETERS;
    }
    
    if(!file_exists(Path)) {
        SnSetLastError(STATUS_FAILURE);
        return STATUS_FAILURE;
    }
        
    ramdisk = Path;
    
    SnSetLastError(STATUS_SUCCESS);
    return STATUS_SUCCESS;
}

SN0W_RETURN
SN0WAPI
SnEnableDfu(
    __in LPVOID Self
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    
    pwndfu = 1;
    
    SnSetLastError(STATUS_SUCCESS);
    return STATUS_SUCCESS;
}

//
// Not So Public API
//

POPENSN0W_CORE_CLASS
SN0WAPI
SnInitializeCore(
    __in USHORT RequestedMajorVersion,
    __in USHORT RequestedMinorVersion
    )
{
    POPENSN0W_CORE_CLASS Class = malloc(sizeof(OPENSN0W_CORE_CLASS));
    
    if(!Class) {
        printf("Failure during allocation of core class memory\n");
        return NULL;
    }
    
    memset(Class, 0, sizeof(OPENSN0W_CORE_CLASS));
    
    Class->Magic = OPENSN0W_CORE_CLASS_MAGIC;
    Class->AbiRevision = SN0W_ABIREV;
    Class->MajorVersion = RequestedMajorVersion;
    Class->MinorVersion = RequestedMinorVersion;
    Class->GetLastError = SnGetLastError;
    Class->GetDeviceChipId = SnGetDeviceChipId;
    Class->GetDeviceSerialNumber = SnGetDeviceSerialNumber;
    Class->GetDeviceCanonicalName = SnGetDeviceCanonicalName;
    Class->SendCommandToDevice = SnSendCommandToDevice;
    Class->SendFileToDevice = SnSendFileToDevice;
    Class->LoadConfigurationFile = SnLoadConfigurationFile;
    Class->FixRecoveryMode = SnFixRecoveryMode;
    Class->SetBootLogoPath = SnSetBootLogoPath;
    Class->SetBundlePath = SnSetBundlePath;
    Class->SetKernelPath = SnSetKernelPath;
    Class->EnableDfuPwn = SnEnableDfu;
    Class->StartJailbreak = SnJailbreak;
    
    return Class;
}

SN0W_RETURN
SN0WAPI
SnDeinitializeCore(
    __in LPVOID Self
    )
{
    POPENSN0W_CORE_CLASS Class = (POPENSN0W_CORE_CLASS)Self;
    __self_integrity_check(Class);
    memset(Self, 0, sizeof(OPENSN0W_CORE_CLASS));
    free(Self);
}