#include "../include/StatesImplementation.h"
#include "../include/Constants.h"
#include <Wire.h>

// --- NormalState ---
void NormalState::enter(Context* ctx) {
    lcd.noBacklight();
    lcd.noDisplay(); // LCD csak proximity-ben aktív
}

void NormalState::update(Context* ctx) {
    // Ellenőrizzük, hogy most van-e etetési idő
    if (Constants::DEBUG_MODE && Constants::SERVO_DEBUG_MODE)
    {
        // Debug módban a gomb állapota határozza meg
        if (digitalRead(Constants::SERVO_BUTTON_PIN)) {
            ctx->setState(new RollupState());
        }        
    }
    else 
    {
        DateTime now = rtc.now();
        bool feedHour = false;        
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
}

// --- RollupState ---
void RollupState::enter(Context* ctx) {
    lcd.clear();
    lcd.print("Rollup...");
    int startAngle = Constants::ROLLDOWN_ANGLE;
    int endAngle = Constants::ROLLUP_ANGLE;

    unsigned long stepDelay = Constants::MOVE_INTERVAL_IN_MILLIS / abs(endAngle - startAngle);
    if (startAngle < endAngle) {
        for (int angle = startAngle; angle <= endAngle; ++angle) {
            feederServo.write(angle);
            delay(stepDelay);
        }
    } else {
        for (int angle = startAngle; angle >= endAngle; --angle) {
            feederServo.write(angle);
            delay(stepDelay);
        }
    }
    ctx->setState(new OpenState());
}
void RollupState::update(Context* ctx) {}

// --- OpenState ---
void OpenState::enter(Context* ctx) {
    lcd.clear();
    lcd.print("Open...");
    startMillis = millis();
}

void OpenState::update(Context* ctx) {
    if (millis() - startMillis >= Constants::OPEN_INTERVAL_IN_MILLIS) {
        ctx->setState(new RolldownState());
    }
}

// --- RolldownState ---
void RolldownState::enter(Context* ctx) {
    lcd.noDisplay();
    int startAngle = Constants::ROLLUP_ANGLE;
    int endAngle = Constants::ROLLDOWN_ANGLE;

    unsigned long stepDelay = Constants::MOVE_INTERVAL_IN_MILLIS / abs(endAngle - startAngle);
    if (startAngle < endAngle) {
        for (int angle = startAngle; angle <= endAngle; ++angle) {
            feederServo.write(angle);
            delay(stepDelay);
        }
    } else {
        for (int angle = startAngle; angle >= endAngle; --angle) {
            feederServo.write(angle);
            delay(stepDelay);
        }
    }
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
    lcd.backlight();
}

void ProximityState::update(Context* ctx) {
    DateTime now = rtc.now();
    lcd.setCursor(0, 1);
    lcd.print(now.hour());
    lcd.print(":");
    if (now.minute() < 10) lcd.print("0");
    lcd.print(now.minute());
}

void ProximityState::onButton1(Context* ctx) {
    ctx->setState(new HourSetState());
}
