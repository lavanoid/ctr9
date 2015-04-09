#ifndef __SDMMC_H__
#define __SDMMC_H__

#include <stdint.h>

void sdmmc_sdcard_init();
int sdmmc_sdcard_readsector(uint32_t sector_no, uint8_t *out);
int sdmmc_sdcard_readsectors(uint32_t sector_no, uint32_t numsectors, uint8_t *out);
int sdmmc_sdcard_writesector(uint32_t sector_no, uint8_t *in);
int sdmmc_sdcard_writesectors(uint32_t sector_no, uint32_t numsectors, uint8_t *in);

int sdmmc_nand_readsectors(uint32_t sector_no, uint32_t numsectors, uint8_t *out);
int sdmmc_nand_writesectors(uint32_t sector_no, uint32_t numsectors, uint8_t *in); // TODO: not implemented

#endif
