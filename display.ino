#include "display.h"

#include <LCDI2C_Generic.h>

namespace {
    LCDI2C_Generic lcd(0x27, 16, 2);
}

void my::Display::Setup() {
    lcd.init();
    lcd.backlight();
}

void my::Display::Print(const char* str) {
    lcd.print(str);
}

void my::Display::Clear() {
    lcd.clear();
}
