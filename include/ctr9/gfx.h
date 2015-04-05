#ifndef __GFX_H__
#define __GFX_H__

#include "types.h"

typedef enum {
    TOP_SCREEN,
    BOTTOM_SCREEN
} Screen;

u32 gfxGetWidth(Screen screen);
u32 gfxGetHeight(Screen screen);
void gfxClearScreen(Screen screen, u8 color);
void gfxClearScreens(u8 color);
void gfxDrawChar(Screen screen, char c, int x, int y);
void gfxDrawString(Screen screen, const char* str, int x, int y);
void gfxPrintString(const char* str);

#endif