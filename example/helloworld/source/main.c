#include "ctr9/3ds.h"

int main() {
    gfxPrintString("Hello World!");
    gfxPrintString("Press START to return to home menu.");

    while(true) {
        hidScanInput();
        if(hidIsPressed(BUTTON_START)) {
            gfxPrintString("Returning to home menu...");
            reboot();
            break;
        }
    }

    return 0;
}