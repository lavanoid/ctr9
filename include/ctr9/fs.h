#ifndef __FS_H__
#define __FS_H__

#include "types.h"

#define OPEN_READ 0x01
#define OPEN_WRITE 0x02
#define OPEN_CREATE_NEW 0x04
#define OPEN_CREATE_ALWAYS 0x08

typedef enum {
    FS_OK = 0,
    FS_DISK_ERR,
    FS_INT_ERR,
    FS_NOT_READY,
    FS_NO_FILE,
    FS_NO_PATH,
    FS_INVALID_NAME,
    FS_DENIED,
    FS_EXIST,
    FS_INVALID_OBJECT,
    FS_WRITE_PROTECTED,
    FS_INVALID_DRIVE,
    FS_NOT_ENABLED,
    FS_NO_FILESYSTEM,
    FS_MKFS_ABORTED,
    FS_TIMEOUT,
    FS_LOCKED,
    FS_NOT_ENOUGH_CORE,
    FS_TOO_MANY_OPEN_FILES,
    FS_INVALID_PARAMETER
} FSResult;

typedef u8 FileHandle[36];

FSResult fsInit();
FSResult fsExit();
FSResult fsOpen(FileHandle* handle, const char* path, u8 flags);
FSResult fsClose(FileHandle* handle);
FSResult fsGetSize(FileHandle* handle, u32* size);
FSResult fsSeek(FileHandle* handle, u32 offset);
FSResult fsRead(FileHandle* handle, u32* bytesRead, void* buf, u32 size);
FSResult fsWrite(FileHandle* handle, u32* bytesWritten, void* buf, u32 size);

void fsReadNANDSectors(u32 sector, u32 sectors, void* buf);
void fsWriteNANDSectors(u32 sector, u32 sectors, void* buf);

#endif