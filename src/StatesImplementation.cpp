#include "StatesImplementation.h"
#include <Wire.h>

// --- NormalState ---
void NormalState::enter(Context* ctx) {
    lcd.clear();
    lcd.print("Normal mode");
}
void NormalState::update(Context* ctx) {
    DateTime now = rtc.now();
    lcd.setCursor(0, 1);
    lcd.print(now.hour());
    lcd.print(":" );
    if (now.minute() < 10) lcd.print("0");
    lcd.print(now.minute());
    // Feeder times: 7:00, 12:00, 19:00
    if ((now.hour() == 7 || now.hour() == 12 || now.hour() == 19) && now.minute() == 0 && now.second() == 0) {
        ctx->setState(new RollupState());
    }
}
void NormalState::onButton1(Context* ctx) {
    ctx->setState(new HourSetState());
}

// --- RollupState ---
void RollupState::enter(Context* ctx) {
    lcd.clear();
    lcd.print("Rollup...");
    feederServo.write(180);
    ctx->setState(new OpenState());
}
void RollupState::update(Context* ctx) {}

// --- OpenState ---
void OpenState::enter(Context* ctx) {
    lcd.clear();
    lcd.print("Open...");
    delay(5000);
    ctx->setState(new RolldownState());
}
void OpenState::update(Context* ctx) {}

// --- RolldownState ---
void RolldownState::enter(Context* ctx) {
    lcd.clear();
    lcd.print("Rolldown...");
    feederServo.write(0);
    ctx->setState(new NormalState());
}
void RolldownState::update(Context* ctx) {}

// --- HourSetState ---
static int setHour = 0, setMinute = 0;
void HourSetState::enter(Context* ctx) {
    DateTime now = rtc.now();
    setHour = now.hour();
    setMinute = now.minute();
    lcd.clear();
    lcd.print("Set Hour: ");
    lcd.print(setHour);
}
void HourSetState::update(Context* ctx) {
    lcd.setCursor(0, 1);
    lcd.print("Hour: ");
    lcd.print(setHour);
}
void HourSetState::onButton1(Context* ctx) {
    ctx->setState(new MinuteSetState());
}
void HourSetState::onButton2(Context* ctx) {
    setHour = (setHour + 1) % 24;
}
void HourSetState::onButton3(Context* ctx) {
    setHour = (setHour + 23) % 24;
}

// --- MinuteSetState ---
void MinuteSetState::enter(Context* ctx) {
    lcd.clear();
    lcd.print("Set Minute: ");
    lcd.print(setMinute);
}
void MinuteSetState::update(Context* ctx) {
    lcd.setCursor(0, 1);
    lcd.print("Minute: ");
    lcd.print(setMinute);
}
void MinuteSetState::onButton1(Context* ctx) {
    rtc.adjust(DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), setHour, setMinute, 0));
    ctx->setState(new NormalState());
}
void MinuteSetState::onButton2(Context* ctx) {
    setMinute = (setMinute + 1) % 60;
}
void MinuteSetState::onButton3(Context* ctx) {
    setMinute = (setMinute + 59) % 60;
}
