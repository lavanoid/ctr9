#include <fatfs/sdmmc.h>
#include "ctr9/fs.h"

#include "fatfs/ff.h"

static FATFS fs;

FSResult fsInit() {
    return (FSResult) f_mount(&fs, "0:", 0);
}

FSResult fsExit() {
    return (FSResult) f_mount(NULL, "0:", 0);
}

FSResult fsOpen(FileHandle* handle, const char* path, u8 flags) {
    FSResult result = (FSResult) f_open((FIL*) handle, path, flags);
    if(result == FS_OK) {
        f_lseek((FIL*) handle, 0);
        f_sync((FIL*) handle);
    }

    return result;
}

FSResult fsClose(FileHandle* handle) {
    return (FSResult) f_close((FIL*) handle);
}

FSResult fsGetSize(FileHandle* handle, u32 *size) {
    *size = (u32) f_size((FIL*) handle);
    return FS_OK;
}

FSResult fsSeek(FileHandle* handle, u32 offset) {
    return (FSResult) f_lseek((FIL*) handle, offset);
}

FSResult fsRead(FileHandle* handle, u32* bytesRead, void* buf, u32 size) {
    return (FSResult) f_read((FIL*) handle, buf, size, (UINT*) bytesRead);
}

FSResult fsWrite(FileHandle* handle, u32* bytesWritten, void* buf, u32 size) {
    FSResult result = (FSResult) f_write((FIL*) handle, buf, size, (UINT*) bytesWritten);
    if(result == FS_OK) {
        f_sync((FIL*) handle);
    }

    return result;
}

void fsReadNANDSectors(u32 sector, u32 sectors, void* buf) {
    sdmmc_nand_readsectors(sector, sectors, buf);
}

void fsWriteNANDSectors(u32 sector, u32 sectors, void* buf) {
    sdmmc_nand_writesectors(sector, sectors, buf);
}