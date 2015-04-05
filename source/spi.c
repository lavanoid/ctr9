#include "ctr9/spi.h"

#include <string.h>

#define IRQ_IE (*(vu32*) 0x10001000)

#define SPI_CNT0 (*(vu16*) 0x10142000)
#define SPI_DATA0 (*(vu8*) 0x10142002)

#define SPI_ENABLE BIT(15)
#define SPI_CONTINUOUS BIT(11)
#define SPI_BUSY BIT(7)

#define SPI_DEVICE_FIRMWARE (1 << 8)

#define FIRMWARE_WREN 0x06
#define FIRMWARE_RDSR 0x05
#define FIRMWARE_READ 0x03
#define FIRMWARE_PW 0x0A

inline void spiWaitBusy() {
    while(SPI_CNT0 & SPI_BUSY);
}

u8 spiReadWrite(u8 data) {
    SPI_DATA0 = data;
    spiWaitBusy();
    return SPI_DATA0;
}

void spiReadNTRFirmware(u32 address, void* destination, u32 size) {
    u32 oldIE = IRQ_IE;
    IRQ_IE = 0;

    u8* buffer = destination;

    SPI_CNT0 = SPI_ENABLE | SPI_CONTINUOUS | SPI_DEVICE_FIRMWARE;
    spiReadWrite(FIRMWARE_READ);
    spiReadWrite((u8) ((address >> 16) & 0xFF));
    spiReadWrite((u8) ((address >> 8) & 0xFF));
    spiReadWrite((u8) (address & 0xFF));

    for(u32 pos = 0; pos < size; pos++) {
        buffer[pos] = spiReadWrite(0);
    }

    SPI_CNT0 = 0;

    IRQ_IE = oldIE;
}

bool spiWriteNTRFirmwarePage(u32 address, u8* buffer) {
    u8 pagebuffer[256];

    spiReadNTRFirmware(address, pagebuffer, 256);
    if(memcmp(pagebuffer, buffer, 256) == 0) {
        return true;
    }

    u32 oldIE = IRQ_IE;
    IRQ_IE = 0;

    SPI_CNT0 = SPI_ENABLE | SPI_CONTINUOUS | SPI_DEVICE_FIRMWARE;
    spiReadWrite(FIRMWARE_WREN);
    SPI_CNT0 = 0;

    SPI_CNT0 = SPI_ENABLE | SPI_CONTINUOUS | SPI_DEVICE_FIRMWARE;
    spiReadWrite(FIRMWARE_RDSR);
    while((spiReadWrite(0) & 0x02) == 0);
    SPI_CNT0 = 0;

    SPI_CNT0 = SPI_ENABLE | SPI_CONTINUOUS | SPI_DEVICE_FIRMWARE;
    spiReadWrite(FIRMWARE_PW);
    spiReadWrite((u8) ((address >> 16) & 0xFF));
    spiReadWrite((u8) ((address >> 8) & 0xFF));
    spiReadWrite((u8) (address & 0xFF));
    for(u32 pos = 0; pos < 256; pos++) {
        spiReadWrite(buffer[pos]);
    }

    SPI_CNT0 = 0;

    SPI_CNT0 = SPI_ENABLE | SPI_CONTINUOUS | SPI_DEVICE_FIRMWARE;
    spiReadWrite(FIRMWARE_RDSR);
    while(spiReadWrite(0) & 0x01);
    SPI_CNT0 = 0;

    IRQ_IE = oldIE;

    spiReadNTRFirmware(address, pagebuffer, 256);
    return memcmp(pagebuffer, buffer, 256) == 0;
}

bool spiWriteNTRFirmware(u32 address, void* source, u32 size) {
    if((address & 0xFF) != 0 || (size & 0xFF) != 0) {
        return -1;
    }

    u8* buffer = source;
    while(size > 0) {
        size -= 256;
        if(!spiWriteNTRFirmwarePage(address + size, buffer + size)) {
            break;
        }
    }

    return size == 0;
}