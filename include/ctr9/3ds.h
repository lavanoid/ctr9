#ifndef __3DS_H__
#define __3DS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "fs.h"
#include "gfx.h"
#include "hid.h"
#include "i2c.h"
#include "ntrcard.h"
#include "spi.h"
#include "types.h"
#include "svc.h"

void reboot();
void powerOff();

#ifdef __cplusplus
}
#endif

#endif