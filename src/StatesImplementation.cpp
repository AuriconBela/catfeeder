#include "../include/StatesImplementation.h"
#include "../include/Constants.h"
#include <Wire.h>
#include "StatesImplementation.h"
#include "../include/Constants.h"

#ifdef DEBUG_MODE
#define DebugMode
#endif

#if defined(DEBUG_MODE) && defined(SERVO_DEBUG_MODE)
#define ServoDebugMode
#endif

namespace {
    //This is a 5th-order polynomial (quintic), which ensures zero velocity and acceleration at both ends—thus finite jerk.
    void moveServoSmooth(bool reverse) {
        const float theta0 = Constants::ROLLUP_ANGLE;  // start angle
        const float theta1 = Constants::ROLLDOWN_ANGLE; // end angle
        const float T = Constants::MOVE_INTERVAL_IN_MILLIS * 0.001; // total duration in seconds
        const float dt = Constants::MOVE_STEP_INTERVAL_IN_MILLIS * 0.001;      // step time in seconds
        const int steps = T / dt;  

        for (int i = 0; i <= steps; i++) {
            float t = i * dt;
            float s = 10 * pow(t/T, 3) - 15 * pow(t/T, 4) + 6 * pow(t/T, 5);
            float theta = reverse
                ? theta1 - s * (theta1 - theta0)
                : theta0 + s * (theta1 - theta0);
            feederServo.write(theta);
            delay(dt * 1000);
        }       
    }

    int GetVoltageDividerValue() {
        int rawValue = analogRead(Constants::VOLTAGE_DIVIDER_PIN);
        float voltage = (rawValue / 1023.0) * 5.0; // Adjusted for 5.0V reference
        float batteryVoltage = voltage * ((Constants::VOLTAGE_DIVIDER_RESISTOR_1 + Constants::VOLTAGE_DIVIDER_RESISTOR_2) / Constants::VOLTAGE_DIVIDER_RESISTOR_2);
        #ifdef DebugMode
        Serial.print("Voltage: ");
        Serial.println(batteryVoltage);
        #endif        
        return round(100 * (batteryVoltage / 3.3) / 5.0) * 5;
    }
}

// --- NormalState ---
void NormalState::enter(Context* ctx) {
    lcd.noBacklight();
    lcd.noDisplay(); // LCD csak proximity-ben aktív
}

void NormalState::update(Context* ctx) {
    // Ellenőrizzük, hogy most van-e etetési idő
    #ifdef ServoDebugMode
    // Debug módban a gomb állapota határozza meg
    if (digitalRead(Constants::SERVO_BUTTON_PIN) == HIGH) {
        Serial.println("Debug: Button pressed, rolling up feeder.");
        ctx->setState(new RolldownState());
    }        
    #else
    DateTime now = rtc.now();
    bool feedHour = false;        
    for (int i = 0; i < 3; ++i) {
        if (now.hour() == Constants::FEED_HOURS[i]) {
            feedHour = true;
            break;
        }
    }
    if (feedHour && now.minute() == 0 && now.second() == 0) {
        ctx->setState(new RolldownState());
    }       
    #endif  
}

// --- RollupState ---
void RollupState::enter(Context* ctx) {
    lcd.clear();
    lcd.print("Rollup...");

    moveServoSmooth(true);

    ctx->setState(new NormalState());
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
        ctx->setState(new RollupState());
    }
}

// --- RolldownState ---
void RolldownState::enter(Context* ctx) {
    lcd.noDisplay();

    moveServoSmooth(false);  

    ctx->setState(new OpenState());
}
void RolldownState::update(Context* ctx) {}

// --- HourSetState ---
static int setHour = 0, setMinute = 0;
void HourSetState::enter(Context* ctx) {
    DateTime now = rtc.now();
    setHour = now.hour();
    setMinute = now.minute();
    lcd.clear();
    lcd.print("Set hour: ");
    lcd.print(setHour);
}
void HourSetState::update(Context* ctx) {
    lcd.setCursor(0, 1);
    lcd.print("Hour: ");
    if (setHour < 10) lcd.print("0");
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
    lcd.print("Set minute: ");
    lcd.print(setMinute);
}
void MinuteSetState::update(Context* ctx) {
    lcd.setCursor(0, 1);
    lcd.print("Minute: ");
    if (setMinute < 10) lcd.print("0");
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
    
    // Battery percentage in top right corner
    int batteryPercent = GetVoltageDividerValue();
    lcd.setCursor(12, 0);
    lcd.print(batteryPercent);
    lcd.print("%");
}

void ProximityState::onButton1(Context* ctx) {
    ctx->setState(new HourSetState());
}

// --- IdleState ---
void IdleState::enter(Context* ctx) {
    lcd.clear();
    lcd.print("Idle mode");
    lcd.noBacklight();
}

void IdleState::update(Context* ctx) {
    // Idle állapotban nem csinálunk semmit
}
