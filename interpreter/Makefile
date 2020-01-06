#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>/devkitARM")
endif

include $(DEVKITARM)/3ds_rules

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH     := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS   := -g -Wall -O2 -mword-relocations -ffunction-sections \
			-Wno-strict-aliasing -fomit-frame-pointer -ffast-math \
			$(ARCH)

CFLAGS   += $(INCLUDE) -DARM11 -D_3DS

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11

ASFLAGS  := -g $(ARCH)

LDFLAGS  = -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS     := -lctru -lm -lz

#---------------------------------------------------------------------------------
# list of directories including libraries, this must be  the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS := $(CTRULIB) $(PORTLIBS)

#---------------------------------------------------------------------------------
# Python metainformation for the build process
#---------------------------------------------------------------------------------

# Set the Python version to compile.
# Defaults to 3.6.9.
PYTHON_VERSION ?= 3.6.9

# Due to misleading newlib headers, these functions
# get detected as present, even though they aren't.
# They will be stubbed out in pyconfig.h.
INVALID_CONFIG_ENTRIES := CHROOT FCHDIR FDATASYNC SETGROUPS SYMLINK

# The name of the statically linked libpython that should be produced.
LIBPYTHON = libpython$(shell echo $(PYTHON_VERSION) | cut -b 1-3).a

# The local directory for the Python source tree.
PYTHON_DIR = Python-$(PYTHON_VERSION)

# The repository to retrieve the interpreter from.
PYTHON_REPOSITORY = https://github.com/vbe0201/cpython

# The repository branch that contains 3DS build patches.
PYTHON_BRANCH = 3ds

# The output directory for CPython compile results.
PYTHON_OUT = cpython-3ds

OUTDIR = libpython

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for Python cross-compilation
#---------------------------------------------------------------------------------
export INCLUDE   := $(foreach dir,$(LIBDIRS),-I$(dir)/include)

export LIBPATHS  := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

# Use gnu-sed if user is on macOS.
ifeq ($(UNAME_S),Darwin)
	SED := gsed
else
	SED := sed
endif

#---------------------------------------------------------------------------------
.PHONY: all clean

all: $(CURDIR)/$(OUTDIR)

$(CURDIR)/$(OUTDIR): $(CURDIR)/$(PYTHON_DIR)/$(LIBPYTHON)
	@mkdir -p $@
	@mkdir -p $@/lib
	cp -r $(CURDIR)/$(PYTHON_DIR)/Include $(CURDIR)/$(OUTDIR)/include
	cp $(CURDIR)/$(PYTHON_DIR)/pyconfig.h $(CURDIR)/$(OUTDIR)/include/
	cp $(CURDIR)/$(LIBPYTHON) $(CURDIR)/$(OUTDIR)/lib/

$(CURDIR)/$(PYTHON_DIR)/$(LIBPYTHON): $(CURDIR)/$(PYTHON_DIR)/Makefile
	$(MAKE) $(MAKEFLAGS) -C $(CURDIR)/$(PYTHON_DIR) LIBRARY="$(LIBPYTHON)" LDLIBRARY="$(LIBPYTHON)" $(LIBPYTHON)
	cp $(CURDIR)/$(PYTHON_DIR)/$(LIBPYTHON) .

$(CURDIR)/$(PYTHON_DIR)/Makefile: $(CURDIR)/$(PYTHON_DIR)
	cp $(CURDIR)/config.site $(PYTHON_DIR)/
	cd $(PYTHON_DIR) && \
	./configure CC="$(CC)" CXX="$(CXX)" AS="$(AS)" AR="$(AR)" OBJCOPY="$(OBJCOPY)" STRIP="$(STRIP)" NM="$(NM)" RANLIB="$(RANLIB)" \
	  CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" ASFLAGS="$(ASFLAGS)" LDFLAGS="$(LDFLAGS) $(LIBPATHS) $(LIBS)" CONFIG_SITE=config.site \
	  --enable-optimizations --without-doc-strings --disable-shared --disable-ipv6 --host=arm-none-eabi --build=`./config.guess` && \
	$(foreach opt,$(INVALID_CONFIG_ENTRIES),`$(SED) -i "s/#define HAVE_$(opt) 1/\/* #undef HAVE_$(opt) *\//" pyconfig.h`) && \
	cd ..

$(CURDIR)/$(PYTHON_DIR):
	git clone $(PYTHON_REPOSITORY) $(PYTHON_DIR) -b $(PYTHON_BRANCH)

clean:
	rm -rf $(OUTDIR) $(PYTHON_DIR) libpython*.a
