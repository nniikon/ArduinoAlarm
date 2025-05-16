#include "encoder.h"

namespace {
    EncButton eb(4, 3, 2);
}

void my::Encoder::Setup() {
    // сбросить счётчик энкодера
    eb.counter = 0;
}

void my::Encoder::StartLoop() {
    eb.tick();

#ifdef DEBUG
    if (eb.turn()) {
        Serial.print("my::Encoder: turn: dir ");
        Serial.print(eb.dir());
        Serial.print(", fast ");
        Serial.print(eb.fast());
        Serial.print(", hold ");
        Serial.print(eb.pressing());
        Serial.print(", counter ");
        Serial.print(eb.counter);
        Serial.print(", clicks ");
        Serial.println(eb.getClicks());
    }

    if (eb.click()) {
        Serial.print("my::Encoder: click: ");
        Serial.println(eb.getClicks());
    }
#endif
}

int my::Encoder::GetDir() {
    return eb.dir();
}

int my::Encoder::GetCounter() {
    return eb.counter;
}

bool my::Encoder::IsPressed() {
    return eb.pressing();
}

bool my::Encoder::IsClick() {
    return eb.click();
}

bool my::Encoder::IsTurn() {
    return eb.turn();
}

bool my::Encoder::IsFast() {
    return eb.fast();
}
