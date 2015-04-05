#ifndef __SPI_H__
#define __SPI_H__

#include "types.h"

void spiReadNTRFirmware(u32 address, void* destination, u32 size);
bool spiWriteNTRFirmware(u32 address, void* source, u32 size);

#endif