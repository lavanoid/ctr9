ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitpro")
endif

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

BUILD_DIR := bin
OUTPUT_DIR := lib
TOOLS_DIR := tools
SOURCES := source
INCLUDES := include
BUILD_FLAGS := -Wall -Os -fomit-frame-pointer -ffast-math

recurse = $(shell find $2 -type $1 -name '$3' 2> /dev/null)

CFILES := $(foreach dir,$(SOURCES),$(call recurse,f,$(dir),*.c))
CPPFILES := $(foreach dir,$(SOURCES),$(call recurse,f,$(dir),*.cpp))
SFILES := $(foreach dir,$(SOURCES),$(call recurse,f,$(dir),*.S))
OBJS := $(addprefix $(BUILD_DIR)/,$(CFILES:.c=.o)) $(addprefix $(BUILD_DIR)/,$(CPPFILES:.cpp=.o)) $(addprefix $(BUILD_DIR)/,$(SFILES:.S=.o))
INCLUDE := $(foreach dir,$(INCLUDES),-I$(dir)) $(foreach dir,$(SOURCES),-I$(dir))

DEVKITARM_BIN := $(DEVKITARM)/bin
PREFIX := $(DEVKITARM_BIN)/arm-none-eabi-
CC := $(PREFIX)gcc
CPP := $(PREFIX)g++
AR := $(PREFIX)ar

ARCH := -mthumb -mthumb-interwork -march=armv5te -mtune=arm946e-s
BASE_FLAGS := $(BUILD_FLAGS) $(ARCH) -I$(INCLUDES)
CFLAGS := $(BASE_FLAGS) -std=gnu99
CPPFLAGS := $(BASE_FLAGS) -fno-rtti -fno-exceptions -std=gnu++11
SFLAGS := $(ARCH)

all: $(OUTPUT_DIR)/libctr9.a

clean:
	@rm -rf $(BUILD_DIR) $(OUTPUT_DIR) ctr9.tar.bz2
	@echo "Cleaned."

install: ctr9.tar.bz2
	@echo "Installing..."
	@mkdir -p $(DEVKITPRO)/ctr9
	@bzip2 -cd ctr9.tar.bz2 | tar -x -C $(DEVKITPRO)/ctr9
	@echo "Installed."

ctr9.tar.bz2: all
	@echo $@
	@tar -cjf ctr9.tar.bz2 $(INCLUDES) $(OUTPUT_DIR) $(TOOLS_DIR)

$(OUTPUT_DIR)/libctr9.a: $(OBJS)
	@echo $@
	@mkdir -p $(dir $@)
	@$(AR) -rcs $@ $(OBJS)

$(BUILD_DIR)/%.o: %.c
	@echo $@
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@echo $@
	@mkdir -p $(dir $@)
	@$(CPP) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S
	@echo $@
	@mkdir -p $(dir $@)
	@$(CC) $(SFLAGS) -c $< -o $@
