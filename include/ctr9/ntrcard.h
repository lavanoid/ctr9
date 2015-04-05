#ifndef __NTRCARD_H__
#define __NTRCARD_H__

#include "types.h"

void ntrcardWriteCommand(const u8 *command);
void ntrcardPolledTransfer(u32 flags, u32 *destination, u32 length, const u8 *command);
void ntrcardParamCommand(u8 command, u32 parameter, u32 flags, u32 *destination, u32 length);
u32 ntrcardRead(u32 flags);
u32 ntrcardWriteAndRead(const u8 *command, u32 flags);

void ntrcardReset();
void ntrcardReadHeader(u8 *header);
u32 ntrcardReadID(u32 flags);

u8 ntrcardEepromCommand(u8 command);
u32 ntrcardEepromReadID();
int ntrcardEepromGetType();
u32 ntrcardEepromGetSize();
void ntrcardEepromRead(u32 address, u8 *data, u32 length, u32 addrtype);
void ntrcardEepromWrite(u32 address, u8 *data, u32 length, u32 addrtype);
void ntrcardEepromChipErase();
void ntrcardEepromSectorErase(u32 address);

#endif