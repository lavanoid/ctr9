#ifndef __I2C_H__
#define __I2C_H__

#include "types.h"

typedef struct {
    u8 bus;
    u8 address;
} I2CDevice;

static const I2CDevice DEVICE_0 = {0, 0x4A};
static const I2CDevice DEVICE_1 = {0, 0x7A};
static const I2CDevice DEVICE_2 = {0, 0x78};
static const I2CDevice DEVICE_MCU = {1, 0x4A};
static const I2CDevice DEVICE_4 = {1, 0x78};
static const I2CDevice DEVICE_5 = {1, 0x2C};
static const I2CDevice DEVICE_6 = {1, 0x2E};
static const I2CDevice DEVICE_7 = {1, 0x40};
static const I2CDevice DEVICE_8 = {1, 0x44};
static const I2CDevice DEVICE_9 = {2, 0xD6};
static const I2CDevice DEVICE_GYROSCOPE = {2, 0xD0};
static const I2CDevice DEVICE_11 = {2, 0xD2};
static const I2CDevice DEVICE_DEBUGPAD = {2, 0xA4};
static const I2CDevice DEVICE_IR = {2, 0x9A};
static const I2CDevice DEVICE_14 = {2, 0xA0};

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

#endif