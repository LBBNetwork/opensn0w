SHARED_LIB	= ${LIB_PREFIX}sn0wcore${LIB_SUFFIX}
LIB		= ${LIB_PREFIX}sn0wcore${LIB_SUFFIX}
LIB_MAJOR	= 1
LIB_MINOR	= 0

BASE_SRCS = \
	arm.c \
	device.c \
	config.c \
	libpartial.c \
	config_file.c \
	libirecovery.c \
	jailbreak.c \
	lzss.c \
	lzssfile.c \
	patch.c \
	plist.c \
	nor_files.c \
	portable_crt.c \
	shell.c \
	util.c \
	loader_posix.c \
	loader_win32.c \
        module.c \
	list.c \
	exploit_list.c \
	abstractfile.c \
	img3.c \
	img2.c \
	8900.c \
	patch_file.c \
	core_objects.c \
	public_api.c

SRCS = ${BASE_SRCS}

include ../extra.mk
include ../buildsys.mk

CPPFLAGS	+= $(MOWGLI_CFLAGS) -I../include -DBINDIR=\"$(bindir)\" -I../include/xpwntool -I../include/libusb-1.0
LIBS		+= $(MOWGLI_LIBS) $(STACKTRACE_LIBS) $(RPATH) $(PROG_IMPLIB_LDFLAGS)

build: all

include .deps
