#include "timer.h"

#include <Arduino.h>

namespace {
    GyverDS3231 rtc;
}

using namespace my;

void Timer::Setup() {
    setStampZone(3);  // часовой пояс
    Wire.begin();
    rtc.begin();

    if (rtc.isReset()) {
        rtc.setBuildTime();
    }

#ifdef DEBUG
    Serial.print("my::Timer: OK: ");
    Serial.println(rtc.isOK());

    Serial.print("my::Timer: Reset: ");
    Serial.println(rtc.isReset());

    LogCurrentTime();
#endif
}

void Timer::LogCurrentTime() {
    Serial.print(rtc.toString());
    Serial.print(':');
    Serial.println(rtc.ms());
}

Datime Timer::GetTime() {
    return rtc;
}

bool Timer::Tick() {
    return rtc.tick();
}


//if (rtc.tick()) {
//    // вывод даты и времени строкой

//    // вывод в Datime
//    Datime dt = rtc;  // или Datime dt(rtc)
//    dt.year;
//    dt.second;
//    dt.hour;
//    dt.weekDay;
//    dt.yearDay;
//    // ... и прочие методы и переменные Datime

//    // сравнение
//    rtc == DaySeconds(12, 35, 0);            // сравнение с DaySeconds (время равно 12:35:00)
//    rtc == 1738237474;                       // сравнение с unix
//    rtc == Datime(2025, 1, 30, 14, 14, 30);  // сравнение с Datime
//}

//Serial.print(rtc.toString());  // rtc.timeToString(), rtc.dateToString()
//Serial.print(':');
//Serial.println(rtc.ms());  // + миллисекунды текущей секунды. Внутри tick всегда равно 0
