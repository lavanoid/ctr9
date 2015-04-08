#include "ctr9/i2c.h"

#define I2C_CNT_STOP BIT(0)
#define I2C_CNT_START BIT(1)
#define I2C_CNT_PAUSE BIT(2)
#define I2C_CNT_ACK BIT(4)
#define I2C_CNT_READ BIT(5)
#define I2C_CNT_IRQ BIT(6)
#define I2C_CNT_BUSY BIT(7)

#define I2C_DATA(device) *((vu8*) (device.bus + 0))
#define I2C_CNT(device) *((vu8*) (device.bus + 1))

static u32 i2cDelay = 0;

void i2cWaitBusy(I2CDevice device) {
    while(I2C_CNT(device) & 0x80);
    for(u32 delay = i2cDelay; delay != 0; delay--) {
        asm volatile("");
    }
}

u8 i2cRead(I2CDevice device) {
    I2C_CNT(device) = I2C_CNT_BUSY | I2C_CNT_IRQ | I2C_CNT_READ;
    i2cWaitBusy(device);
    return I2C_DATA(device);
}

bool i2cWrite(I2CDevice device, u8 data, u8 extraFlags) {
    I2C_DATA(device) = data;
    I2C_CNT(device) = (u8) (I2C_CNT_BUSY | I2C_CNT_IRQ | extraFlags);
    i2cWaitBusy(device);
    return (bool) (I2C_CNT(device) & I2C_CNT_ACK);
}

void i2cStop(I2CDevice device) {
    I2C_CNT(device) = I2C_CNT_BUSY | I2C_CNT_IRQ | I2C_CNT_PAUSE | I2C_CNT_STOP;
    i2cWaitBusy(device);
    i2cDelay = 0;
}

bool i2cStart(I2CDevice device, I2CRegister reg, bool read) {
    if(!i2cWrite(device, device.address, I2C_CNT_START)) {
        i2cStop(device);
        return false;
    }

    if(!i2cWrite(device, reg, 0)) {
        i2cStop(device);
        return false;
    }

    if(read) {
        if(!i2cWrite(device, (u8) (device.address | 1), I2C_CNT_START)) {
            i2cStop(device);
            return false;
        }
    }

    if(device.bus == DEVICE_MCU.bus && device.address == DEVICE_MCU.address) {
        i2cDelay = 0x180;
    } else {
        i2cDelay = 0;
    }

    return true;
}

u8 i2cReadRegister(I2CDevice device, I2CRegister reg) {
    u8 ret = 0xFF;
    bool success = false;
    for(u32 attempts = 0; attempts < 8 && !success; attempts++) {
        success = i2cStart(device, reg, true);
        if(success) {
            ret = i2cRead(device);
        }

        i2cStop(device);
    }

    return ret;
}

bool i2cWriteRegister(I2CDevice device, I2CRegister reg, u8 data) {
    bool success = false;
    for(u32 attempts = 0; attempts < 8 && !success; attempts++) {
        success = i2cStart(device, reg, false) && i2cWrite(device, data, 0);
        i2cStop(device);
    }

    return success;
}