ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitpro")
endif

BUILD_DIR := bin
OUTPUT_DIR := output
SOURCES := source
INCLUDES := include
LIBRARIES := $(DEVKITPRO)/ctr9
BUILD_FLAGS := -Wall -Os -fomit-frame-pointer -ffast-math

include $(DEVKITPRO)/ctr9/tools/make_base
