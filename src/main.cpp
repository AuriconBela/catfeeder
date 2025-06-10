#include <Arduino.h>
#include <Servo.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
#include "../include/Context.h"
#include "../include/StatesImplementation.h"

Servo feederServo;
RTC_DS3231 rtc;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int button1Pin = 6, button2Pin = 7, button3Pin = 8;
int proximityPin = 10;

Context ctx;
bool lastProximity = false;

void setup() {
    feederServo.attach(9);
    lcd.begin(16, 2);
    rtc.begin();
    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
    pinMode(button3Pin, INPUT_PULLUP);
    pinMode(proximityPin, INPUT);
    lcd.noDisplay(); // LCD alapból kikapcsolva
    ctx.setState(new NormalState());
}

void loop() {
    bool proximity = digitalRead(proximityPin) == HIGH; // vagy LOW, ha fordított a logika
    if (proximity && !lastProximity) {
        // Csak akkor lépjen proximity state-be, ha engedélyezett az átmenet
        State* s = ctx.getState();
        if (dynamic_cast<NormalState*>(s) || dynamic_cast<RolldownState*>(s)) {
            ctx.setState(new ProximityState());
        }
    } else if (!proximity && lastProximity) {
        // Csak akkor lépjen vissza normal state-be, ha proximity state-ben van
        State* s = ctx.getState();
        if (dynamic_cast<ProximityState*>(s)) {
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
    delay(50);
}
