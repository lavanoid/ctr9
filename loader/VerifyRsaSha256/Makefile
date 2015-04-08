ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

BUILD_DIR := bin
OUTPUT_DIR := output
SOURCES := source
INCLUDES := include

CFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
SFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.S))
OBJS := $(addprefix $(BUILD_DIR)/,$(CFILES:.c=.o)) $(addprefix $(BUILD_DIR)/,$(SFILES:.S=.o))

DEVKITARM_BIN := $(DEVKITARM)/bin
BIN2S := $(DEVKITARM_BIN)/bin2s
PREFIX := $(DEVKITARM_BIN)/arm-none-eabi
CC := $(PREFIX)-gcc
LD := $(PREFIX)-ld
OBJCOPY := $(PREFIX)-objcopy

ARCH := -march=armv5te -mtune=arm946e-s
ASFLAGS := $(ARCH)
CFLAGS := -Os -Wall -std=gnu99 $(ASFLAGS) -I$(INCLUDES)
LDFLAGS := -nostartfiles -nostdlib

KEY := 580006192800C5F0FBFB04E06A682088
IV := 00000000000000000000000000000000

all: $(OUTPUT_DIR)/Launcher.dat

clean:
	@rm -rf $(BUILD_DIR) $(OUTPUT_DIR)
	@echo "Cleaned."

$(OUTPUT_DIR)/Launcher.dat: $(OUTPUT_DIR)/MsetForBoss.dat
	@echo $@
	@mkdir -p $(dir $@)
	@openssl enc -aes-128-cbc -K $(KEY) -iv $(IV) -in $< -out $@

$(OUTPUT_DIR)/MsetForBoss.dat: $(OUTPUT_DIR)/payload.bin
	@echo $@
	@mkdir -p $(dir $@)
	@python2 tools/3dsploit.py $< $@

$(OUTPUT_DIR)/payload.bin: $(BUILD_DIR)/payload.elf
	@echo $@
	@mkdir -p $(dir $@)
	@$(OBJCOPY) -S -O binary $< $@

$(BUILD_DIR)/payload.elf: $(OBJS)
	@echo $@
	@mkdir -p $(dir $@)
	@$(LD) $(LDFLAGS) -T tools/linker.x $(OBJS) -o $@

$(BUILD_DIR)/%.o: %.c
	@echo $@
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S
	@echo $@
	@mkdir -p $(dir $@)
	@$(CC) $(ASFLAGS) -c $< -o $@
