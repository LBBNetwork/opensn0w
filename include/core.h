/*
 * Core routines
 */

#ifndef _core_h_
#define _core_h_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <ctype.h>
#include <wctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>

#include "libirecovery.h"
#include "libpartial.h"
#include "warning.h"
#include "config.h"
#include "abirev.h"
#include "glib_shim.h"
#include "version.h"
#include "xpwntool/common.h"
#include "xpwntool/xpwn/plist.h"
#include "config_file.h"
#include "patch.h"
#include "platform.h"
#include "crt.h"
#include "module.h"
#include "util.h"
#include "exploits.h"
#include "firmware.h"
#include "messages/usa.h"
#include "dprint.h"



#if !defined(_WIN32) && !defined(__OS2__)
# ifndef __HPUX__
#  define PLATFORM_SUFFIX ".so"
# else
#  define PLATFORM_SUFFIX ".sl"
# endif
#else
# define PLATFORM_SUFFIX ".dll"
#endif

#ifdef __APPLE__
#undef PLATFORM_SUFFIX
#define PLATFORM_SUFFIX ".impl"
#endif

int is_compatible(void);
char *mode_to_string(int mode);
int poll_device(int mode);
int send_command(char *name);
int send_file(char *name);

int jailbreak(void);
Dictionary *createDictionaryFromBuffer(char* buffer, int len);
DictValue *getNextKey(Dictionary * myself);
int irecovery_shell_initialize();
extern irecv_device_t device;
extern irecv_client_t client;

#define usage(x) \
	printf(USAGE_MESSAGE, \
            "opensn0w", \
			"s5l8930x, s5l8920x, s5l8922x", "s5l8720x, s5l8900x", "opensn0w", "opensn0w"); \
			exit(-1);


#ifndef BUFSIZE
#define BUFSIZE             256
#endif

/* device.c */


#endif
