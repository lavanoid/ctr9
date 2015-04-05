#include "ctr9/3ds.h"

void reboot() {
    i2cWriteRegister(DEVICE_MCU, MCU_POWER, MCU_POWER_REBOOT);
}

void powerOff() {
    i2cWriteRegister(DEVICE_MCU, MCU_POWER, MCU_POWER_OFF);
}