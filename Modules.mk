hwdb_modules = klogger
build_modules = $(hwdb_modules)	# Modules to build

klogger_sources = \
	hashtable.c \
	hwdbflow.c \
	klogger.c

klogger_headers = \
	hashtable.h \
	hwdbflow.h \
	khwdb.h \
	debug.h \
	klogger.h

build_sources = $(foreach module,$(build_modules),$($(module)_sources))
build_headers = $(foreach module,$(build_modules),$($(module)_headers))

build_links = $(notdir $(build_sources))

build_objects = $(notdir $(patsubst %.c,%.o,$(build_sources)))

