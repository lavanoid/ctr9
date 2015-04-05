#include "ctr9/i2c.h"

#define I2C_DATA 0
#define I2C_CNT 1

#define I2C_CNT_STOP BIT(0)
#define I2C_CNT_START BIT(1)
#define I2C_CNT_PAUSE BIT(2)
#define I2C_CNT_ACK BIT(4)
#define I2C_CNT_READ BIT(5)
#define I2C_CNT_IRQ BIT(6)
#define I2C_CNT_BUSY BIT(7)

static u32 bus[] = {
        0x10161000,
        0x10144000,
        0x10148000,
};

vu8* i2cGetDataReg(I2CDevice device) {
    return (vu8*) (bus[device.bus] + I2C_DATA);
}

vu8* i2cGetCntReg(I2CDevice device) {
    return (vu8*) (bus[device.bus] + I2C_CNT);
}

inline void i2cWaitBusy(I2CDevice device) {
    while(*i2cGetCntReg(device) & 0x80);
}

u8 i2cRead(I2CDevice device) {
    i2cWaitBusy(device);
    *i2cGetCntReg(device) = I2C_CNT_BUSY | I2C_CNT_IRQ | I2C_CNT_READ;
    i2cWaitBusy(device);
    return *i2cGetDataReg(device);
}

bool i2cWrite(I2CDevice device, u8 data, u8 extraFlags) {
    i2cWaitBusy(device);
    *i2cGetDataReg(device) = data;
    *i2cGetCntReg(device) = (u8) (I2C_CNT_BUSY | I2C_CNT_IRQ | extraFlags);
    i2cWaitBusy(device);
    return (bool) (*i2cGetCntReg(device) & I2C_CNT_ACK);
}

void i2cStop(I2CDevice device) {
    i2cWaitBusy(device);
    *i2cGetCntReg(device) = I2C_CNT_BUSY | I2C_CNT_IRQ | I2C_CNT_PAUSE | I2C_CNT_STOP;
    i2cWaitBusy(device);
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