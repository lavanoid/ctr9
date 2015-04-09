#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

void spiReadNTRFirmware(u32 address, void* destination, u32 size);
bool spiWriteNTRFirmware(u32 address, void* source, u32 size);

#ifdef __cplusplus
};
#endif

#endif