#include "display.h"

#include <LCDI2C_Russian.h>

namespace {
    LCDI2C_Russian lcd(0x27, 16, 2);
}

void my::Display::Setup() {
    lcd.init();
    lcd.backlight();
}

void my::Display::Print(const char* str) {
    lcd.println(str);
}

void my::Display::Clear() {
    lcd.clear();
}
