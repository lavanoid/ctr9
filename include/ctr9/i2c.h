#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

typedef struct {
    u32 bus;
    u8 address;
} I2CDevice;

static const I2CDevice DEVICE_0 = {0x10161000, 0x4A};
static const I2CDevice DEVICE_1 = {0x10161000, 0x7A};
static const I2CDevice DEVICE_2 = {0x10161000, 0x78};
static const I2CDevice DEVICE_MCU = {0x10144000, 0x4A};
static const I2CDevice DEVICE_4 = {0x10144000, 0x78};
static const I2CDevice DEVICE_5 = {0x10144000, 0x2C};
static const I2CDevice DEVICE_6 = {0x10144000, 0x2E};
static const I2CDevice DEVICE_7 = {0x10144000, 0x40};
static const I2CDevice DEVICE_8 = {0x10144000, 0x44};
static const I2CDevice DEVICE_9 = {0x10148000, 0xD6};
static const I2CDevice DEVICE_GYROSCOPE = {0x10148000, 0xD0};
static const I2CDevice DEVICE_11 = {0x10148000, 0xD2};
static const I2CDevice DEVICE_DEBUGPAD = {0x10148000, 0xA4};
static const I2CDevice DEVICE_IR = {0x10148000, 0x9A};
static const I2CDevice DEVICE_14 = {0x10148000, 0xA0};

typedef enum {
    MCU_VOLUME = 0x09,
    MCU_BATTERY = 0x0B,
    MCU_POWER = 0x20
} I2CRegister;

#define MCU_VOLUME_MAX 0x3F
#define MCU_BATTERY_MAX 0x3F
#define MCU_POWER_REBOOT 0x4
#define MCU_POWER_OFF 0x8

u8 i2cReadRegister(I2CDevice device, I2CRegister reg);
bool i2cWriteRegister(I2CDevice device, I2CRegister reg, u8 data);

#ifdef __cplusplus
};
#endif

#endif