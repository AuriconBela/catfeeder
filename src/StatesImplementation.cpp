#include "../include/StatesImplementation.h"
#include "../include/Constants.h"
#include <Wire.h>

// --- NormalState ---
void NormalState::enter(Context* ctx) {
    lcd.noDisplay(); // LCD csak proximity-ben aktív
}
void NormalState::update(Context* ctx) {
    DateTime now = rtc.now();
    bool feedHour = false;
    // Ellenőrizzük, hogy most van-e etetési idő
    for (int i = 0; i < 3; ++i) {
        if (now.hour() == Constants::FEED_HOURS[i]) {
            feedHour = true;
            break;
        }
    }
    if (feedHour && now.minute() == 0 && now.second() == 0) {
        ctx->setState(new RollupState());
    }
}

// --- RollupState ---
void RollupState::enter(Context* ctx) {
    lcd.clear();
    lcd.print("Rollup...");
    feederServo.write(Constants::ROLLUP_ANGLE);
    ctx->setState(new OpenState());
}
void RollupState::update(Context* ctx) {}

// --- OpenState ---
void OpenState::enter(Context* ctx) {
    lcd.clear();
    lcd.print("Open...");
    delay(Constants::OPEN_INTERVAL_IN_MILLIS); // Várakozás az etetésre
    ctx->setState(new RolldownState());
}
void OpenState::update(Context* ctx) {}

// --- RolldownState ---
void RolldownState::enter(Context* ctx) {
    lcd.noDisplay();
    feederServo.write(Constants::ROLLDOWN_ANGLE);
    // Átmenet proximity vagy normal state-be, ezt main.cpp dönti el proximity alapján
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
    ctx->setState(new ProximityState());
}
void MinuteSetState::onButton2(Context* ctx) {
    setMinute = (setMinute + 1) % 60;
}
void MinuteSetState::onButton3(Context* ctx) {
    setMinute = (setMinute + 59) % 60;
}

// --- ProximityState ---
void ProximityState::enter(Context* ctx) {
    lcd.display();
    lcd.clear();
    lcd.print("Hello cat!");
}
void ProximityState::update(Context* ctx) {
    // Itt lehetne animáció vagy info
}
void ProximityState::onButton1(Context* ctx) {
    ctx->setState(new HourSetState());
}
