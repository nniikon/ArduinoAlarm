#include <Arduino.h>
#include <stdio.h>

#include "timer.h"
#include "encoder.h"
#include "display.h"
#include "player.h"

my::Timer timer;
my::Encoder encoder;
my::Display display;
my::Player player;

struct AlarmTime {
    int hour;
    int minute;
};

enum AlarmMode {
    TimeDisplay = 0,
    HourSelect  = 1,
    MinuteSelect = 2,
    SetUnsetSelect = 3,
    AlarmModeCounter
};

static constexpr bool kDebugEnabled = true;

static void setupDebug();
static void displayCurrentTime(const Datime& now);
static void displayAlarmSetting();
static void adjustAlarmValue(int& value, int minVal, int maxVal);
static bool isBlinkTime();

void setup() {
    Serial.begin(9600);
    setupDebug();

    timer.Setup();
    encoder.Setup();
    display.Setup();
    player.Setup();

    pinMode(A2, OUTPUT);
    digitalWrite(A2, HIGH);

    display.Print("Setup end!");
}

void loop() {
    static AlarmTime alarm_time = {0, 0};
    static AlarmMode alarmMode = TimeDisplay;
    static bool is_alarm_ringing = false;
    static bool is_alarm_set     = false;

    static unsigned long tick_aftertime = 0;

    encoder.StartLoop();

    Datime now = timer.GetTime();

    bool tick = timer.Tick();
    bool blink_tick = isBlinkTime();

    bool is_click    = encoder.IsClick();
    bool is_turn     = encoder.IsTurn();
    bool is_fast     = encoder.IsFast();
    int  encoder_dir = encoder.GetDir();

    if (is_click) {
        tick_aftertime = 50;
    }
    else if (tick_aftertime > 0) {
        tick_aftertime--;
    }

    if (is_click) {
        alarmMode = static_cast<AlarmMode>((alarmMode + 1) % (AlarmModeCounter));
    }

    if (alarmMode == TimeDisplay && tick) {
        displayCurrentTime(now, alarm_time, is_alarm_set);
    }

    else if (alarmMode != TimeDisplay && (tick || is_turn || is_click)) {
        bool blink_hour = (tick_aftertime > 0 && alarmMode == HourSelect);
        bool blink_min  = (tick_aftertime > 0 && alarmMode == MinuteSelect);
        bool blink_set  = (tick_aftertime > 0 && alarmMode == SetUnsetSelect);
        displayAlarmSetting(alarm_time, blink_hour, blink_min, blink_set, is_alarm_set);
    }

    if (is_turn) {
        switch (alarmMode) {
            case HourSelect:
                adjustAlarmHour(&alarm_time.hour, is_fast, encoder_dir);
                break;
            case MinuteSelect:
                adjustAlarmMinutes(&alarm_time.minute, &alarm_time.hour, is_fast, encoder_dir);
                break;
            case SetUnsetSelect:
                is_alarm_set = !is_alarm_set;
                break;
            default:
                break;
        }
    }

    if (alarmMode == TimeDisplay && now.hour == alarm_time.hour &&
                                    now.minute == alarm_time.minute &&
                                    is_alarm_set) {
        if (!is_alarm_ringing) {
            player.Play(1);
            Serial.println("Playing alarm!");
            is_alarm_ringing = true;
        }

        is_alarm_set = false;
    }

    if (is_alarm_ringing) {
        if (!isBulbBlinkTime()) {
            digitalWrite(A2, HIGH);
        }
        else {
            digitalWrite(A2, LOW);
        }
    }

    if (is_turn) {
        if (is_alarm_ringing) {
            player.Stop();
            digitalWrite(A2, HIGH);
            Serial.println("Not playing alarm!");
            is_alarm_ringing = false;
        }
    }
}

static bool isBulbBlinkTime() {
    const unsigned long kBlinkOffInterval = 800;
    const unsigned long kBlinkOnInterval  = 800;

    static bool is_blink = true;

    static unsigned long last_blink_time = 0;
    unsigned long        current_time    = millis();

    unsigned long interval = is_blink ? kBlinkOnInterval : kBlinkOffInterval;
    if (current_time - last_blink_time >= interval) {
        last_blink_time = current_time;
        is_blink = !is_blink;
    }

    return !is_blink;
}

static bool isBlinkTime() {
    const unsigned long kBlinkOffInterval = 100;
    const unsigned long kBlinkOnInterval  = 1000;

    static bool is_blink = true;

    static unsigned long last_blink_time = 0;
    unsigned long        current_time    = millis();

    unsigned long interval = is_blink ? kBlinkOnInterval : kBlinkOffInterval;
    if (current_time - last_blink_time >= interval) {
        last_blink_time = current_time;
        is_blink = !is_blink;
    }

    return !is_blink;
}

static void setupDebug() {
    if (kDebugEnabled) {
        Serial.println("--------------------------------------------------");
        Serial.println("Program Started (DEBUG MODE)");
        Serial.println("--------------------------------------------------");
    }
}

static void displayCurrentTime(const Datime& now, const AlarmTime& alarm_time, bool is_alarm_set) {
    display.Clear();

    char row1[17] = {};
    char row2[17] = {};
    char alarm_str[7] = {};
    char alarm_time_str[7] = {};

    if (is_alarm_set) { snprintf(alarm_str, sizeof(alarm_str), " ALARM"); }
    else              { snprintf(alarm_str, sizeof(alarm_str), " SLEEP"); }

    if (is_alarm_set) { snprintf(alarm_time_str, sizeof(alarm_time_str), " %02d:%02d", alarm_time.hour, alarm_time.minute); }
    else              { snprintf(alarm_time_str, sizeof(alarm_time_str), "      "); }
    //                            123  456  789 0123456
    snprintf(row1, sizeof(row1), "%02d:%02d:%02d|%s", now.hour, now.minute, now.second,     alarm_str);
    snprintf(row2, sizeof(row2), "%02d.%02d.%02d|%s", now.day,  now.month,  now.year % 100, alarm_time_str);

    display.Print(row1);
    display.Print(row2);
}

static void displayAlarmSetting(const AlarmTime& alarm_time,
                                bool is_blink_hour,
                                bool is_blink_minute,
                                bool is_blink_set,
                                bool is_alarm_set) {
    char row1[17] = {};
    char row2[17] = {};
    char hourStr[3]   = {};
    char minuteStr[3] = {};
    char setStr[6]    = {};

    if (is_blink_hour) snprintf(hourStr, sizeof(hourStr), "  ");
    else               snprintf(hourStr, sizeof(hourStr), "%02d", alarm_time.hour);

    if (is_blink_minute) snprintf(minuteStr, sizeof(minuteStr), "  ");
    else                 snprintf(minuteStr, sizeof(minuteStr), "%02d", alarm_time.minute);

    if      (is_blink_set) snprintf(setStr, sizeof(setStr), "     ");
    else if (is_alarm_set) snprintf(setStr, sizeof(setStr), "ALARM");
    else                   snprintf(setStr, sizeof(setStr), "SLEEP");

    snprintf(row1, sizeof(row1), "Alarm Time|%s", setStr);
    snprintf(row2, sizeof(row2), "----|%s:%s|-----", hourStr, minuteStr);

    display.Clear();

    display.Print(row1);
    display.Print(row2);
}

const int kDayHours = 24;
const int kDayMinutes = 60;

static void adjustAlarmHour(int* hour, bool is_fast, int dir) {

    if (dir == 0) {
        Serial.println("adjustAlarmHour: dir == 0 (WTF?)");
        return;
    }

    int delta = is_fast ? 5 : 1;

    if (dir > 0) {
        *hour = (*hour + delta) % kDayHours;
        // Ensure the result is positive
        if (*hour < 0) {
            *hour += kDayHours;
        }
    } else {
        *hour = (*hour - delta + kDayHours) % kDayHours;
    }
}

static void adjustAlarmMinutes(int* minutes, int* hours, bool is_fast, int dir) {

    if (dir == 0) {
        Serial.println("adjustAlarmMinutes: dir == 0 (WTF?)");
        return;
    }

    int delta = is_fast ? 10 : 1;

    if (dir > 0) {
        *minutes += delta;
        *hours += *minutes / kDayMinutes;
        *minutes %= kDayMinutes;
    } else {
        *minutes -= delta;
        if (*minutes < 0) {
            int borrow = (-*minutes + (kDayMinutes - 1)) / kDayMinutes;
            *hours -= borrow;
            *minutes += borrow * kDayMinutes;
        }
    }

    *hours = (*hours % kDayHours + kDayHours) % kDayHours;
}
