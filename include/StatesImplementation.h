#ifndef STATES_IMPLEMENTATION_H
#define STATES_IMPLEMENTATION_H

#include "State.h"
#include "Context.h"
#include <Servo.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include "Constants.h"

// Forward declarations for hardware
extern Servo feederServo;
extern RTC_DS3231 rtc;
extern LiquidCrystal_I2C lcd;
extern int button1Pin, button2Pin, button3Pin;

// --- State Classes ---
// Állapotok: normal, proximity, rollup, open, rolldown, hour set, minute set
class NormalState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
    StateType getType() const override { return NORMAL; }
    unsigned long getLoopDelay() const override { return Constants::LOOP_END_DELAY; }
};

class ProximityState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
    void onButton1(Context* ctx) override;
    StateType getType() const override { return PROXIMITYSTATE; }
    unsigned long getLoopDelay() const override { return Constants::LOOP_END_DELAY; }
};

class IdleState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
    StateType getType() const override { return IDLE; }
    unsigned long getLoopDelay() const override { return Constants::IDLE_STATE_DELAY; }
};

class RollupState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
    StateType getType() const override { return ROLLUP; }
    unsigned long getLoopDelay() const override { return Constants::LOOP_END_DELAY; }
};

class OpenState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
    StateType getType() const override { return OPEN; }
    unsigned long getLoopDelay() const override { return Constants::LOOP_END_DELAY; }
private:
    unsigned long startMillis = 0;
};

class RolldownState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
    StateType getType() const override { return ROLLDOWN; }
    unsigned long getLoopDelay() const override { return Constants::LOOP_END_DELAY; }
};

class HourSetState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
    void onButton1(Context* ctx) override;
    void onButton2(Context* ctx) override;
    void onButton3(Context* ctx) override;
    StateType getType() const override { return HOUR_SET; }
    unsigned long getLoopDelay() const override { return Constants::LOOP_END_DELAY_SHORT; }
};

class MinuteSetState : public State {
public:
    void enter(Context* ctx) override;
    void update(Context* ctx) override;
    void onButton1(Context* ctx) override;
    void onButton2(Context* ctx) override;
    void onButton3(Context* ctx) override;
    StateType getType() const override { return MINUTE_SET; }
    unsigned long getLoopDelay() const override { return Constants::LOOP_END_DELAY_SHORT; }
};

#endif // STATES_IMPLEMENTATION_H
