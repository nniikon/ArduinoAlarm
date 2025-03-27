#include <stdio.h>
#include <Arduino.h>

#include "timer.h"
my::Timer timer;

#include "encoder.h"
my::Encoder encoder;

#include "display.h"
my::Display display;

struct AlarmTime {
    int hour;
    int minute;
};

AlarmTime alarmTime = {0, 0};

const int ALARM_MODE_TIME = 0;
const int ALARM_MODE_HOUR_CHOOSE = 1;
const int ALARM_MODE_MINUTE_CHOOSE = 2;

int alarmChooseMode = ALARM_MODE_TIME;

#define DEBUG

void SetupDebugMode() {
#ifdef DEBUG
    Serial.print("--------------------------------------------------""\n");
    Serial.print("Program Started (DEBUG MODE)"                      "\n");
    Serial.print("--------------------------------------------------""\n");
#endif
}

void setup() {
    Serial.begin(9600);

    SetupDebugMode();

    timer.Setup();
    encoder.Setup();
    display.Setup();

    display.Print("Hello World!");
}

void DisplayTime(const Datime& time) {
    display.Clear();

    char first_row[16] = {0};
    char second_row[16] = {0};

    snprintf(first_row, sizeof(first_row),   "%02d:%02d:%02d|      ", time.hour, time.minute, time.second);
    snprintf(second_row, sizeof(second_row), "%02d.%02d.%02d|      ", time.day, time.month, time.year % 100);

    display.Print(first_row);
    display.Print(second_row);
}

void DisplayChooseAlarm() {
    display.Clear();
    display.Print("   Alarm Time:  ");
    char second_row[16] = {0};
    snprintf(second_row, sizeof(second_row), "     %02d:%02d     ", alarmTime.hour, alarmTime.minute);
    display.Print(second_row);
}

void loop() {
    encoder.StartLoop();

    if (encoder.IsClick()) {
        alarmChooseMode = (alarmChooseMode + 1) % 3;
    }

    Datime time = timer.GetTime();

    bool timer_tick = timer.Tick();

    if (timer_tick && alarmChooseMode == ALARM_MODE_TIME)
        DisplayTime(time);
    else if ((timer_tick || encoder.IsTurn()) && alarmChooseMode != ALARM_MODE_TIME)
        DisplayChooseAlarm();

    if (alarmChooseMode == ALARM_MODE_HOUR_CHOOSE && encoder.IsTurn()) {
        if (encoder.GetDir() == 1) {
            if (alarmTime.hour < 23) {
                alarmTime.hour += encoder.IsFast() ? 5 : 1;
            }
        }
        if (encoder.GetDir() == -1) {
            if (alarmTime.hour > 0) {
                alarmTime.hour -= encoder.IsFast() ? 5 : 1;
            }
        }
    }

    if (alarmChooseMode == ALARM_MODE_MINUTE_CHOOSE && encoder.IsTurn()) {
        if (encoder.GetDir() == 1) {
            if (alarmTime.minute < 59) {
                alarmTime.minute += encoder.IsFast() ? 5 : 1;
            }
        }
        if (encoder.GetDir() == -1) {
            if (alarmTime.minute > 0) {
                alarmTime.minute -= encoder.IsFast() ? 5 : 1;
            }
        }
    }

    //SetEncoderCounter();
    //IsEncoderKeyPressed();
}
