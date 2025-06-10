#include <Arduino.h>
#include <Servo.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
#include "Context.h"
#include "StatesImplementation.h"

Servo feederServo;
RTC_DS3231 rtc;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int button1Pin = 6, button2Pin = 7, button3Pin = 8;

Context ctx;

void setup() {
    feederServo.attach(9);
    lcd.begin(16, 2);
    rtc.begin();
    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
    pinMode(button3Pin, INPUT_PULLUP);
    ctx.setState(new NormalState());
}

void loop() {
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
