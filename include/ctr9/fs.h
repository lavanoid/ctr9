#ifndef __FS_H__
#define __FS_H__

#include "types.h"

#define OPEN_READ 0x1
#define OPEN_WRITE 0x6

typedef u8 FileHandle[32];

u32 fileOpen(void* handle, const u16* name, u32 mode);
void fileClose(void* handle);
void fileRead(void* handle, u32* bytesRead, void* buf, u32 size);
void fileWrite(void* handle, u32* bytesWritten, void* buf, u32 size);
void fileGetSize(void* handle, u32* size);

#endif