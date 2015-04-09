#ifndef __HID_H__
#define __HID_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

typedef enum {
    BUTTON_A = BIT(0),
    BUTTON_B  = BIT(1),
    BUTTON_SELECT = BIT(2),
    BUTTON_START = BIT(3),
    BUTTON_RIGHT = BIT(4),
    BUTTON_LEFT = BIT(5),
    BUTTON_UP = BIT(6),
    BUTTON_DOWN = BIT(7),
    BUTTON_R = BIT(8),
    BUTTON_L = BIT(9),
    BUTTON_X = BIT(10),
    BUTTON_Y = BIT(11)
} Button;

void hidScanInput();
bool hidIsHeld(Button button);
bool hidIsPressed(Button button);
bool hidIsReleased(Button button);

#ifdef __cplusplus
};
#endif

#endif