#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>/devkitARM")
endif

TOPDIR ?= $(CURDIR)
include $(DEVKITARM)/3ds_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# INTERPRETER is the location of the Python interpreter port
# LIBPYTHON is the directory containing the cpython include and lib files
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing header files
#
# NO_SMDH: if set to anything, no SMDH file is generated
# APP_TITLE is the name of the app stored in the SMDH file (Optional)
# APP_DESCRIPTION is the description of the app stored in the SMDH file (Optional)
# APP_AUTHOR is the author of the app stored in the SMDH file (Optional)
# ICON is the filename of the icon (.png), relative to the project folder
# DIST_DIR is the directory where all the binaries should be output on make dist
#---------------------------------------------------------------------------------
TARGET          := 3DS.py
BUILD           := build
INTERPRETER     := interpreter
LIBPYTHON       := libpython
SOURCES         := source
INCLUDES        := include
APP_TITLE       := $(TARGET)
APP_DESCRIPTION := Python on the 3DS
APP_AUTHOR      := Valentin B., Python Software Foundation
APP_VERSION     := 0.1.0
ICON            := $(TOPDIR)/icon.png
DIST_DIR        := $(TARGET)-$(APP_VERSION)

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH     := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS   := $(ARCH) -g -Wall -O2 -mword-relocations \
			-fomit-frame-pointer -ffunction-sections

CFLAGS   += $(INCLUDE) -DARM11 -D_3DS #-D_3DS_PY_DEBUG

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11

ASFLAGS  := -g $(ARCH)
LDFLAGS  =  -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS     := -lpython3.6 -lctru -lm -lz

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS  := $(PORTLIBS) $(CTRULIB) $(TOPDIR)/$(BUILD)/libpython

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT   := $(CURDIR)/$(TARGET)
export TOPDIR   := $(CURDIR)

export VPATH    := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR  := $(CURDIR)/$(BUILD)

CFILES          :=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))

export LD       :=  $(CC)

export OFILES   :=  $(CFILES:.c=.o)

export INCLUDE  :=  $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

export _3DSXDEPS    := $(if $(NO_SMDH),,$(OUTPUT).smdh)

ifeq ($(strip $(NO_SMDH)),)
	export _3DSXFLAGS += --smdh=$(CURDIR)/$(TARGET).smdh
endif

.PHONY: $(BUILD) clean all

#---------------------------------------------------------------------------------
all: $(BUILD)

$(BUILD): cpython
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

cpython:
	make --no-print-directory -C $(INTERPRETER)
	mkdir -p $(BUILD)/$(LIBPYTHON)
	cp -r $(INTERPRETER)/$(LIBPYTHON)/* $(BUILD)/$(LIBPYTHON)

dist: $(BUILD)
	mkdir -p $(DIST_DIR)
	cp $(OUTPUT).3dsx $(DIST_DIR)/
	cp $(CURDIR)/main.py $(DIST_DIR)/
	mkdir -p $(DIST_DIR)/lib/python3.6
	unzip $(INTERPRETER)/$(LIBPYTHON)/python_lib.zip -d $(DIST_DIR)/lib/python3.6
	zip -r $(DIST_DIR).zip $(DIST_DIR)

ifneq ($(DEPSDIR),$(BUILD))
$(DEPSDIR):
	@mkdir -p $@
endif

#---------------------------------------------------------------------------------
clean:
	$(MAKE) -C $(INTERPRETER) clean
	@rm -fr $(BUILD) $(OUTPUT).3dsx $(OUTPUT).smdh $(OUTPUT).elf $(OUTPUT)-$(APP_VERSION)*

#---------------------------------------------------------------------------------
else

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).3dsx	:	$(OUTPUT).elf $(_3DSXDEPS)

$(OUTPUT).elf	: $(OFILES)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
