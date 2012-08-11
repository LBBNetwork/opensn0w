-include extra.mk
-include buildsys.mk

SUBDIRS=libsn0wcore modules src
CLEANDIRS = ${SUBDIRS}

buildsys.mk:
	@echo "You need to run ./configure first."
	@exit 1

install-extra:
	@echo "----------------------------------------------------------------"
	@echo ">>> Remember to cd to ${prefix} and edit your config file.";
	@echo "----------------------------------------------------------------"

