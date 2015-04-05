#include "ctr9/hid.h"

#define HID_PAD 0x10146000

u32 currState = 0;
u32 prevState = 0;

void hidScanInput() {
    prevState = currState;
    currState = *((u32*) HID_PAD);
}

bool hidIsHeld(Button button) {
    return !(currState & button);
}

bool hidWasHeld(Button button) {
    return !(prevState & button);
}

bool hidIsPressed(Button button) {
    return hidIsHeld(button) && !hidWasHeld(button);
}

bool hidIsReleased(Button button) {
    return !hidIsHeld(button) && hidWasHeld(button);
}