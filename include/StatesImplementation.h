#ifndef STATES_IMPLEMENTATION_H
#define STATES_IMPLEMENTATION_H

#include "State.h"
#include "Context.h"
#include <Servo.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

// Forward declarations for hardware
extern Servo feederServo;
extern RTC_DS3231 rtc;
extern LiquidCrystal lcd;
extern int button1Pin, button2Pin, button3Pin;

// --- State Classes ---
// Állapotok: normal, proximity, rollup, open, rolldown, hour set, minute set
class NormalState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
    void onButton1(Context* ctx) override;
};

class ProximityState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
    void onButton1(Context* ctx) override;
};

class RollupState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
};

class OpenState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
};

class RolldownState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
};

class HourSetState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
    void onButton1(Context* ctx) override;
    void onButton2(Context* ctx) override;
    void onButton3(Context* ctx) override;
};

class MinuteSetState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
    void onButton1(Context* ctx) override;
    void onButton2(Context* ctx) override;
    void onButton3(Context* ctx) override;
};

#endif // STATES_IMPLEMENTATION_H
