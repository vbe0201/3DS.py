#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

TOPDIR ?= $(CURDIR)
include $(DEVKITARM)/3ds_rules

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH     := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS   := $(ARCH) -g -Wall -O2 -mword-relocations -ffunction-sections \
			-Wno-strict-aliasing -fomit-frame-pointer -ffast-math

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11

ASFLAGS  := -g $(ARCH)

LDFLAGS  =  -g $(ARCH) -Wl,-Map,$(notdir $*.map)

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
PATCH_DIR := ../python_patches

# Use gnu-sed if we're on macOS
ifeq ($(UNAME_S),Darwin)
	SED   := gsed
else
	SED   := sed
endif

LIBDIRS	  := $(PORTLIBS) $(CTRULIB)

export INCLUDE := $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                    -I$(CURDIR)/$(BUILD)

CFLAGS   += $(INCLUDE) -DARM11 -D_3DS -DWITH_THREAD

ifeq ($(strip $(PYVERSION)),)
	PYVERSION := 3.6.9
endif

ANAME     := libpython$(shell echo $(PYVERSION) | cut -b 1-3).a
#---------------------------------------------------------------------------------
# PYDIR is the Python source directory
# PYURL is the download URL for Python
# OUTDIR is the output directory
#---------------------------------------------------------------------------------
PYDIR  := Python-$(PYVERSION)
PYURL  := https://www.python.org/ftp/python/$(PYVERSION)/$(PYDIR).tgz
OUTDIR := libpython

#---------------------------------------------------------------------------------

.PHONY: all clean

all: distPython

distPython: makePython
	@[ -d "$(OUTDIR)" ] || mkdir -p $(OUTDIR)
	@[ -d "$(OUTDIR)/include" ] || mkdir -p $(OUTDIR)/include
	@[ -d "$(OUTDIR)/lib" ] || mkdir -p $(OUTDIR)/lib
	cp $(ANAME) $(OUTDIR)/lib
	cp -r $(PYDIR)/Include/* $(OUTDIR)/include
	cp $(PYDIR)/pyconfig.h $(OUTDIR)/include
	cd $(PYDIR)/Lib && zip -r ../../$(OUTDIR)/python_lib.zip .

makePython: patchPython
	cd $(PYDIR) && \
	$(MAKE) $(MAKEFLAGS) LIBRARY="$(ANAME)" LDLIBRARY="$(ANAME)" $(ANAME) && \
	cd .. && \
	cp $(PYDIR)/$(ANAME) .

patchPython: configurePython
	cp $(PATCH_DIR)/condvars_ctr.h $(PYDIR)/Python/
	cp $(PATCH_DIR)/thread_ctr.h $(PYDIR)/Python/
	patch $(PYDIR)/Python/fileutils.c $(PATCH_DIR)/fileutils.c.diff
	patch $(PYDIR)/pyconfig.h $(PATCH_DIR)/pyconfig.h.diff
	patch $(PYDIR)/Makefile $(PATCH_DIR)/Makefile.diff
	patch $(PYDIR)/Modules/posixmodule.c $(PATCH_DIR)/posixmodule.c.diff
	patch $(PYDIR)/Python/ceval_gil.h $(PATCH_DIR)/ceval_gil.h.diff
	patch $(PYDIR)/Python/condvar.h $(PATCH_DIR)/condvar.h.diff
	patch $(PYDIR)/Python/pytime.c $(PATCH_DIR)/pytime.c.diff
	patch $(PYDIR)/Python/random.c $(PATCH_DIR)/random.c.diff
	patch $(PYDIR)/Python/thread.c $(PATCH_DIR)/thread.c.diff
	$(SED) -e 's/#_thread\s_threadmodule.c/_thread\s_threadmodule.c/g' $(PYDIR)/Modules/Setup.config
	$(SED) -i '167s/_POSIX_THREADS/_3DS/' $(PYDIR)/Modules/_tracemalloc.c

configurePython: preparePython
	cd $(PYDIR) && \
	./configure CC="$(CC)" CXX="$(CXX)" AS="$(AS)" AR="$(AR)" OBJCOPY="$(OBJCOPY)" STRIP="$(STRIP)" NM="$(NM)" RANLIB="$(RANLIB)" \
	  CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" ASFLAGS="$(ASFLAGS)" LDFLAGS="$(LDFLAGS)" CONFIG_SITE=config.site \
	  --enable-optimizations --without-doc-strings --disable-shared --disable-ipv6 --host=arm-none-eabi --build=`./config.guess` && \
	cd ..

preparePython: $(PYDIR)
	$(SED) -i -e 's/\*\-\*\-linux\*)/\*\-\*\-linux\*\|arm\-none\-eabi)/g' $(PYDIR)/configure
	cp $(PATCH_DIR)/config.site $(PYDIR)/
	cp -r $(PATCH_DIR)/_ctru/ $(PYDIR)/Modules/
	cp $(PATCH_DIR)/Setup.dist $(PYDIR)/Modules/

$(PYDIR):
	wget $(PYURL) || curl -L $(PYURL)
	tar xfzv $@.tgz

clean:
	@rm -rf $(OUTDIR) $(PYDIR) $(PYDIR).tgz libpython*.a
