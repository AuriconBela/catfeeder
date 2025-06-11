#include <Arduino.h>
#include <Servo.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include "../include/Context.h"
#include "../include/StatesImplementation.h"

#define DebugMode true

Servo feederServo;
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 is a common I2C address, 16x2 display
int button1Pin = 6, button2Pin = 7, button3Pin = 8;
int proximityPin = 10;

Context ctx;
bool lastProximity = false;

void setup() {
    feederServo.attach(9);
    lcd.init(); // Use init() for LiquidCrystal_I2C
    lcd.backlight(); // Optionally turn on backlight
    rtc.begin();
    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
    pinMode(button3Pin, INPUT_PULLUP);
    pinMode(proximityPin, INPUT);
    lcd.noDisplay(); // LCD alapból kikapcsolva
    ctx.setState(new NormalState());
    #ifdef DebugMode
    Serial.begin(9600);
    #endif    
}

void loop() {
    bool proximity = digitalRead(proximityPin) == HIGH; // vagy LOW, ha fordított a logika
    #ifdef DebugMode
    Serial.println("Proximity: " + String(proximity));
    Serial.println(rtc.now().timestamp(DateTime::TIMESTAMP_FULL));
    #endif      
    if (proximity && !lastProximity) {
        // Csak akkor lépjen proximity state-be, ha engedélyezett az átmenet
        State* s = ctx.getState();
        if (s->getType() == State::NORMAL || s->getType() == State::ROLLDOWN) {
            ctx.setState(new ProximityState());
        }
    } else if (!proximity && lastProximity) {
        // Csak akkor lépjen vissza normal state-be, ha proximity state-ben van
        State* s = ctx.getState();
        if (s->getType() == State::PROXIMITY) {
            ctx.setState(new NormalState());
        }
    }
    lastProximity = proximity;
    ctx.update();
    if (digitalRead(button1Pin) == LOW) {
        delay(200); // debounce
        ctx.onButton1();
    }
    if (digitalRead(button2Pin) == LOW) {
        delay(200);
        ctx.onButton2();
    }
    if (digitalRead(button3Pin) == LOW) {
        delay(200);
        ctx.onButton3();
    }
    delay(1000);
    #ifdef DebugMode
    Serial.println("Current State: " + String(ctx.getState()->getType()));
    #endif
}
