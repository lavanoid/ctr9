#include "ctr9/ntrcard.h"

#define NTRCARD_MCNT (*(vu16*) 0x10164000)
#define NTRCARD_MDATA (*(vu8*) 0x10164002)
#define NTRCARD_ROMCNT (*(vu32*) 0x10164004)
#define NTRCARD_CMD ((vu8*) 0x10164008)
#define NTRCARD_SEEDX_L (*(vu32*) 0x10164010)
#define NTRCARD_SEEDY_L (*(vu32*) 0x10164014)
#define NTRCARD_SEEDX_H (*(vu8*) 0x10164018)
#define NTRCARD_SEEDY_H (*(vu8*) 0x1016401A)
#define NTRCARD_FIFO (*(vu32*) 0x1016401C)

#define NTRCARD_MCNT_ENABLE BIT(15)
#define NTRCARD_MCNT_IRQ BIT(14)
#define NTRCARD_MCNT_SPI_ENABLE BIT(13)
#define NTRCARD_MCNT_SPI_BUSY BIT(7)
#define NTRCARD_MCNT_SPI_HOLD BIT(6)

#define NTRCARD_MDATA_EEPROM_WRSR 0x01
#define NTRCARD_MDATA_EEPROM_PP 0x02
#define NTRCARD_MDATA_EEPROM_READ 0x03
#define NTRCARD_MDATA_EEPROM_WRDI 0x04
#define NTRCARD_MDATA_EEPROM_RDSR 0x05
#define NTRCARD_MDATA_EEPROM_WREN 0x06
#define NTRCARD_MDATA_EEPROM_PW 0x0A
#define NTRCARD_MDATA_EEPROM_FAST 0x0B
#define NTRCARD_MDATA_EEPROM_RDID 0x9F
#define NTRCARD_MDATA_EEPROM_RDP 0xAB
#define NTRCARD_MDATA_EEPROM_DPD 0xB9
#define NTRCARD_MDATA_EEPROM_SE 0xD8

#define NTRCARD_ROMCNT_BUSY BIT(31)
#define NTRCARD_ROMCNT_WR BIT(30)
#define NTRCARD_ROMCNT_nRESET BIT(29)
#define NTRCARD_ROMCNT_SEC_LARGE BIT(28)
#define NTRCARD_ROMCNT_CLK_SLOW BIT(27)
#define NTRCARD_ROMCNT_BLK_SIZE(n) (((n) & 0x7) << 24)
#define NTRCARD_ROMCNT_DATA_READY BIT(23)
#define NTRCARD_ROMCNT_SEC_CMD BIT(22)
#define NTRCARD_ROMCNT_DELAY2(n) (((n) & 0x3F) << 16)
#define NTRCARD_ROMCNT_SEC_SEED BIT(15)
#define NTRCARD_ROMCNT_SEC_EN BIT(14)
#define NTRCARD_ROMCNT_SEC_DAT BIT(13)
#define NTRCARD_ROMCNT_DELAY1(n) ((n) & 0x1FFF)

#define NTRCARD_CMD_DUMMY 0x9F
#define NTRCARD_CMD_HEADER_READ 0x00
#define NTRCARD_CMD_HEADER_CHIPID 0x90
#define NTRCARD_CMD_ACTIVATE_BF 0x3C
#define NTRCARD_CMD_ACTIVATE_SEC 0x40
#define NTRCARD_CMD_SECURE_CHIPID 0x10
#define NTRCARD_CMD_SECURE_READ 0x20
#define NTRCARD_CMD_DISABLE_SEC 0x60
#define NTRCARD_CMD_DATA_MODE 0xA0
#define NTRCARD_CMD_DATA_READ 0xB7
#define NTRCARD_CMD_DATA_CHIPID 0xB8

void ntrcardWriteCommand(const u8* command) {
    NTRCARD_MCNT = NTRCARD_MCNT_ENABLE | NTRCARD_MCNT_IRQ;
    for(u32 index = 0; index < 8; index++) {
        NTRCARD_CMD[7 - index] = command[index];
    }
}

void ntrcardPolledTransfer(u32 flags, u32* destination, u32 length, const u8* command) {
    ntrcardWriteCommand(command);
    NTRCARD_ROMCNT = flags;

    u32* target = destination + length;
    do {
        if(NTRCARD_ROMCNT & NTRCARD_ROMCNT_DATA_READY) {
            u32 data = NTRCARD_FIFO;
            if(destination < target) {
                *destination = data;
            }

            destination++;
        }
    } while(NTRCARD_ROMCNT & NTRCARD_ROMCNT_BUSY);
}

void ntrcardParamCommand(u8 command, u32 parameter, u32 flags, u32* destination, u32 length) {
    ntrcardPolledTransfer(flags, destination, length, (u8[8]) {(u8) command, (u8) (parameter >> 24), (u8) (parameter >> 16), (u8) (parameter >> 8), (u8) (parameter >> 0), 0, 0, 0});
}

u32 ntrcardRead(u32 flags) {
    NTRCARD_ROMCNT = flags | NTRCARD_ROMCNT_BUSY | NTRCARD_ROMCNT_nRESET | NTRCARD_ROMCNT_BLK_SIZE(7);
    while(!(NTRCARD_ROMCNT & NTRCARD_ROMCNT_DATA_READY));
    return NTRCARD_FIFO;
}

u32 ntrcardWriteAndRead(const u8* command, u32 flags) {
    ntrcardWriteCommand(command);
    return ntrcardRead(flags);
}

void ntrcardReset() {
    NTRCARD_ROMCNT = 0;
    NTRCARD_MCNT = 0;

    NTRCARD_MCNT = NTRCARD_MCNT_ENABLE | NTRCARD_MCNT_IRQ;
    NTRCARD_ROMCNT = NTRCARD_ROMCNT_nRESET | NTRCARD_ROMCNT_SEC_SEED;
    while(NTRCARD_ROMCNT & NTRCARD_ROMCNT_BUSY);

    ntrcardWriteCommand((u8[8]) {0, 0, 0, 0, 0, 0, 0, NTRCARD_CMD_DUMMY});
    NTRCARD_ROMCNT = (u32) (NTRCARD_ROMCNT_BUSY | NTRCARD_ROMCNT_nRESET | NTRCARD_ROMCNT_CLK_SLOW | NTRCARD_ROMCNT_BLK_SIZE(5) | NTRCARD_ROMCNT_DELAY2(0x18));

    u32 read = 0;
    do {
        if(NTRCARD_ROMCNT & NTRCARD_ROMCNT_DATA_READY) {
            if(read < 0x2000) {
                u32 data = NTRCARD_FIFO;
                (void) data;
                read += 4;
            }
        }
    } while(NTRCARD_ROMCNT & NTRCARD_ROMCNT_BUSY);
}

void ntrcardReadHeader(u8* header) {
    ntrcardReset();
    ntrcardParamCommand(NTRCARD_CMD_HEADER_READ, 0, (u32) (NTRCARD_ROMCNT_BUSY | NTRCARD_ROMCNT_nRESET | NTRCARD_ROMCNT_CLK_SLOW | NTRCARD_ROMCNT_BLK_SIZE(1) | NTRCARD_ROMCNT_DELAY1(0x1FFF) | NTRCARD_ROMCNT_DELAY2(0x3F)), (u32*) header, 512 / 4);
}

u32 ntrcardReadID(u32 flags) {
    return ntrcardWriteAndRead((u8[8]) {0, 0, 0, 0, 0, 0, 0, NTRCARD_CMD_HEADER_CHIPID}, flags);
}

void ntrcardEepromWaitBusy() {
    while(NTRCARD_MCNT & NTRCARD_MCNT_SPI_BUSY);
}

u8 ntrcardEepromCommand(u8 command) {
    NTRCARD_MCNT = NTRCARD_MCNT_ENABLE | NTRCARD_MCNT_SPI_ENABLE | NTRCARD_MCNT_SPI_HOLD;
    NTRCARD_MDATA = command;
    ntrcardEepromWaitBusy();

    NTRCARD_MDATA = 0;
    ntrcardEepromWaitBusy();

    u8 ret = NTRCARD_MDATA;
    NTRCARD_MCNT = NTRCARD_MCNT_SPI_HOLD;
    return ret;
}

u32 ntrcardEepromReadID() {
    NTRCARD_MCNT = NTRCARD_MCNT_ENABLE | NTRCARD_MCNT_SPI_ENABLE | NTRCARD_MCNT_SPI_HOLD;
    NTRCARD_MDATA = NTRCARD_MDATA_EEPROM_RDID;
    ntrcardEepromWaitBusy();

    u32 id = 0;
    for(u32 i = 0; i < 3; i++) {
        NTRCARD_MDATA = 0;
        ntrcardEepromWaitBusy();
        id = (id << 8) | NTRCARD_MDATA;
    }

    NTRCARD_MCNT = NTRCARD_MCNT_SPI_HOLD;
    return id;
}

int ntrcardEepromGetType() {
    u8 sr = ntrcardEepromCommand(NTRCARD_MDATA_EEPROM_RDSR);
    u32 id = ntrcardEepromReadID();
    if((sr == 0xff && id == 0xffffff) || (sr == 0 && id == 0)) {
        return -1;
    } else if(sr == 0xf0 && id == 0xffffff) {
        return 1;
    } else if(sr == 0x00 && id == 0xffffff) {
        return 2;
    } else if(id != 0xffffff) {
        return 3;
    }

    return 0;
}

u32 ntrcardEepromGetSize() {
    int type = ntrcardEepromGetType();
    if(type == -1) {
        return 0;
    } else if(type == 0) {
        return 8192;
    } else if(type == 1) {
        return 512;
    } else if(type == 2) {
        u32 buf1;
        u32 buf2;
        u32 buf3;
        ntrcardEepromRead(0, (u8 *) &buf1, 4, (u32) type);
        if(!(buf1 != 0 || buf1 != 0xffffffff)) {
            buf3 = ~buf1;
            ntrcardEepromWrite(0, (u8 *) &buf3, 4, (u32) type);
        } else {
            buf3 = buf1;
        }

        u32 size = 8192;
        while(1) {
            ntrcardEepromRead(size, (u8 *) &buf2, 4, (u32) type);
            if(buf2 == buf3) {
                break;
            }

            size += 8192;
        }

        if(buf1 != buf3) {
            ntrcardEepromWrite(0, (u8 *) &buf1, 4, (u32) type);
        }

        return size;
    } else if(type == 3) {
        u32 id = ntrcardEepromReadID();
        u32 device = id & 0xffff;
        if(((id >> 16) & 0xff) == 0x20) {
            switch(device) {
                case 0x4014:
                    return 1024 * 1024;
                case 0x4013:
                case 0x8013:
                    return 512 * 1024;
                case 0x2017:
                    return 8 * 1024 * 1024;
                default:
                    break;
            }
        } else if(((id >> 16) & 0xff) == 0x62) {
            if(device == 0x1100) {
                return 512 * 1024;
            }
        } else if(((id >> 16) & 0xff) == 0xC2) {
            if(device == 0x2211) {
                return 128 * 1024;
            }
        }

        return 256 * 1024;
    }

    return 0;
}

void ntrcardEepromRead(u32 address, u8 *data, u32 length, u32 addrtype) {
    NTRCARD_MCNT = NTRCARD_MCNT_ENABLE | NTRCARD_MCNT_SPI_ENABLE | NTRCARD_MCNT_SPI_HOLD;
    NTRCARD_MDATA = (u8) (NTRCARD_MDATA_EEPROM_READ | ((addrtype == 1) ? address >> 8 << 3 : 0));
    ntrcardEepromWaitBusy();

    if(addrtype == 3) {
        NTRCARD_MDATA = (u8) ((address >> 16) & 0xFF);
        ntrcardEepromWaitBusy();
    }

    if(addrtype >= 2) {
        NTRCARD_MDATA = (u8) ((address >> 8) & 0xFF);
        ntrcardEepromWaitBusy();
    }

    NTRCARD_MDATA = (u8) ((address) & 0xFF);
    ntrcardEepromWaitBusy();

    while(length > 0) {
        NTRCARD_MDATA = 0;
        ntrcardEepromWaitBusy();
        *data++ = NTRCARD_MDATA;
        length--;
    }

    ntrcardEepromWaitBusy();
    NTRCARD_MCNT = NTRCARD_MCNT_SPI_HOLD;
}

void ntrcardEepromWrite(u32 address, u8 *data, u32 length, u32 addrtype) {
    u32 address_end = address + length;
    u32 maxblocks = 32;
    if(addrtype == 1) {
        maxblocks = 16;
    } else if(addrtype == 2) {
        maxblocks = 32;
    } else if(addrtype == 3) {
        maxblocks = 256;
    }

    while(address < address_end) {
        NTRCARD_MCNT = NTRCARD_MCNT_ENABLE | NTRCARD_MCNT_SPI_ENABLE | NTRCARD_MCNT_SPI_HOLD;
        NTRCARD_MDATA = NTRCARD_MDATA_EEPROM_WREN;
        ntrcardEepromWaitBusy();
        NTRCARD_MCNT = NTRCARD_MCNT_SPI_HOLD;

        NTRCARD_MCNT = NTRCARD_MCNT_ENABLE | NTRCARD_MCNT_SPI_ENABLE | NTRCARD_MCNT_SPI_HOLD;
        if(addrtype == 1) {
            NTRCARD_MDATA = (u8) (NTRCARD_MDATA_EEPROM_PP | (address & BIT(8)) >> (8-3));
            ntrcardEepromWaitBusy();
            NTRCARD_MDATA = (u8) (address & 0xFF);
            ntrcardEepromWaitBusy();
        } else if(addrtype == 2) {
            NTRCARD_MDATA = NTRCARD_MDATA_EEPROM_PP;
            ntrcardEepromWaitBusy();
            NTRCARD_MDATA = (u8) (address >> 8);
            ntrcardEepromWaitBusy();
            NTRCARD_MDATA = (u8) (address & 0xFF);
            ntrcardEepromWaitBusy();
        } else if(addrtype == 3) {
            NTRCARD_MDATA = NTRCARD_MDATA_EEPROM_PP;
            ntrcardEepromWaitBusy();
            NTRCARD_MDATA = (u8) ((address >> 16) & 0xFF);
            ntrcardEepromWaitBusy();
            NTRCARD_MDATA = (u8) ((address >> 8) & 0xFF);
            ntrcardEepromWaitBusy();
            NTRCARD_MDATA = (u8) (address & 0xFF);
            ntrcardEepromWaitBusy();
        }

        for(u32 i = 0; address < address_end && i < maxblocks; i++, address++) {
            NTRCARD_MDATA = *data++;
            ntrcardEepromWaitBusy();
        }

        NTRCARD_MCNT = NTRCARD_MCNT_SPI_HOLD;

        NTRCARD_MCNT = NTRCARD_MCNT_ENABLE | NTRCARD_MCNT_SPI_ENABLE | NTRCARD_MCNT_SPI_HOLD;
        NTRCARD_MDATA = NTRCARD_MDATA_EEPROM_RDSR;
        ntrcardEepromWaitBusy();
        do {
            NTRCARD_MDATA = 0;
            ntrcardEepromWaitBusy();
        } while(NTRCARD_MDATA & NTRCARD_MDATA_EEPROM_WRSR);
        ntrcardEepromWaitBusy();
        NTRCARD_MCNT = NTRCARD_MCNT_SPI_HOLD;
    }
}

void ntrcardEepromChipErase() {
    u32 sz = ntrcardEepromGetSize();
    for(u32 sector = 0; sector < sz; sector += 0x10000) {
        ntrcardEepromSectorErase(sector);
    }
}

void ntrcardEepromSectorErase(u32 address) {
    NTRCARD_MCNT = NTRCARD_MCNT_ENABLE | NTRCARD_MCNT_SPI_ENABLE | NTRCARD_MCNT_SPI_HOLD;
    NTRCARD_MDATA = NTRCARD_MDATA_EEPROM_WREN;
    ntrcardEepromWaitBusy();
    NTRCARD_MCNT = NTRCARD_MCNT_SPI_HOLD;

    NTRCARD_MCNT = NTRCARD_MCNT_ENABLE | NTRCARD_MCNT_SPI_ENABLE | NTRCARD_MCNT_SPI_HOLD;
    NTRCARD_MDATA = NTRCARD_MDATA_EEPROM_SE;
    ntrcardEepromWaitBusy();
    NTRCARD_MDATA = (u8) ((address >> 16) & 0xFF);
    ntrcardEepromWaitBusy();
    NTRCARD_MDATA = (u8) ((address >> 8) & 0xFF);
    ntrcardEepromWaitBusy();
    NTRCARD_MDATA = (u8) (address & 0xFF);
    ntrcardEepromWaitBusy();
    NTRCARD_MCNT = NTRCARD_MCNT_SPI_HOLD;

    NTRCARD_MCNT = NTRCARD_MCNT_ENABLE | NTRCARD_MCNT_SPI_ENABLE | NTRCARD_MCNT_SPI_HOLD;
    NTRCARD_MDATA = NTRCARD_MDATA_EEPROM_RDSR;
    ntrcardEepromWaitBusy();
    do {
        NTRCARD_MDATA = 0;
        ntrcardEepromWaitBusy();
    } while(NTRCARD_MDATA & NTRCARD_MDATA_EEPROM_WRSR);
    NTRCARD_MCNT = NTRCARD_MCNT_SPI_HOLD;
}