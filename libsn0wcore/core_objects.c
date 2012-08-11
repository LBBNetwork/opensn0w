/*
 * Core libsn0w objects.
 */

#include "core.h"

char endianness = 1;
int Img3DecryptLast = 1;
int arch = 0;
int global_version = 0;
patch_list_t *iboot_patches;
patch_list_t *kernel_patches;
extern config_file_t *config;
irecv_device_t device = NULL;
irecv_client_t client = NULL;

int opensn0w_debug_level = 0;

char *kernelcache = NULL;
char *bootlogo = NULL;
char *url = NULL;
char *plist = NULL;
char *ramdisk = NULL;
char *config_file = SYSCONFDIR "/opensn0w.conf";
int iboot = false;
int dry_run = false;
int gp_payload = false;
int nologo = false;
int pwndfu = false;
int pwnrecovery = false;
int autoboot = false;
int download = false;
int use_shatter = false;
int do_jailbreak = false;
int dump_bootrom = false;
volatile int jailbreaking = false;