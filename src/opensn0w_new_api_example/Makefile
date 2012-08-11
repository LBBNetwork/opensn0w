PROG		= opensn0w_new_api_example

BASE_SRCS = \
	main.c

SRCS = ${BASE_SRCS}

include ../../extra.mk
include ../../buildsys.mk

CPPFLAGS	+= $(MOWGLI_CFLAGS) -I../../include -DBINDIR=\"$(bindir)\" -I../../include/xpwntool -I../../include/libusb-1.0
LIBS		+= $(MOWGLI_LIBS) $(STACKTRACE_LIBS) $(RPATH) $(PROG_IMPLIB_LDFLAGS) -L../../libsn0wcore -lsn0wcore ${LDFLAGS_RPATH}

build: all

install-extra:
	-$(INSTALL) -m 755 -d $(DESTDIR)$(prefix)
	-$(INSTALL) -m 755 -d $(DESTDIR)$(bindir)
	-$(INSTALL) -m 755 -d $(DESTDIR)$(sysconfdir)
	-$(INSTALL) -m 755 -d $(DESTDIR)$(localstatedir)
	-$(INSTALL) -m 755 -d $(DESTDIR)$(prefix)/bundles
	-$(INSTALL) -m 755 -d $(DESTDIR)$(DOCDIR)
	-$(INSTALL) -m 755 -d $(DESTDIR)$(LOGDIR)
	-$(INSTALL) -m 755 -d $(DESTDIR)$(RUNDIR)
	-$(INSTALL) -m 755 -d $(DESTDIR)$(DATADIR)
	-$(INSTALL) -m 755 -d $(DESTDIR)$(SHAREDIR)/help
	$(INSTALL) -m 600 -c ../../bundles/* $(DESTDIR)$(prefix)/bundles
	$(INSTALL) -m 600 -c ../../dist/opensn0w.conf $(DESTDIR)$(sysconfdir)

include .deps
