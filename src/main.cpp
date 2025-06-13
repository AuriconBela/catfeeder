#include <Arduino.h>
#include <Servo.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <APDS9930.h>
#include "../include/Context.h"
#include "../include/StatesImplementation.h"
#include "../include/ProximityTransitionManager.h"
#include "../include/Constants.h"

#define DebugMode Constants::DEBUG_MODE

Servo feederServo;
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(Constants::LCD_ADDR, 
                      Constants::LCD_COLS, 
                      Constants::LCD_ROWS);
int button1Pin = Constants::BUTTON1_PIN, 
    button2Pin = Constants::BUTTON2_PIN, 
    button3Pin = Constants::BUTTON3_PIN;

APDS9930 apds = APDS9930();
Context ctx;
bool lastProximity = false;
bool isProximityEnabled = false;

ProximityTransitionManager proximityManager;

void setup() {
    #ifdef DebugMode
    Serial.begin(Constants::SERIAL_BAUD_RATE);
    // Wait for Serial to be ready
    while (!Serial) {
        delay(Constants::WAIT_TIME_IN_MILLIS); 
    }    
    #endif      

    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
    pinMode(button3Pin, INPUT_PULLUP);
    // Initialize I2C communication
    Wire.begin(); 
    // Initialize APDS-9930 (configure I2C and initial values)
    if (apds.init()){
        isProximityEnabled = apds.setProximityGain(PGAIN_2X) && apds.enableProximitySensor(false);
    }
  
    feederServo.attach(Constants::SERVO_PIN);
    lcd.init(); // Use init() for LiquidCrystal_I2C
    lcd.backlight(); // Optionally turn on backlight
    rtc.begin();
    
    ctx.setState(new NormalState()); 
}

void loop() {
    bool proximity = false;
    if (isProximityEnabled) {
        uint16_t proxValue = 0;
        apds.readProximity(proxValue);
        proximity = (proxValue > Constants::PROXIMITY_THRESHOLD);

        #ifdef DebugMode
        Serial.print("Proximity: ");
        Serial.println(proxValue);
        #endif

        proximityManager.updateBuffer(proximity);
        ProximityTransitionManager::Transition transition = proximityManager.checkTransition(lastProximity, ctx.getState());
        if (transition == ProximityTransitionManager::TO_PROXIMITY) {
            ctx.setState(new ProximityState());
            lastProximity = true;
        } else if (transition == ProximityTransitionManager::TO_NORMAL) {
            ctx.setState(new NormalState());
            lastProximity = false;
        }        
    }

    ctx.update();
    if (digitalRead(button1Pin) == LOW) {
        delay(Constants::DEBOUNCE_WAIT_IN_MILLIS); // debounce
        ctx.onButton1();
    }
    if (digitalRead(button2Pin) == LOW) {
        delay(Constants::DEBOUNCE_WAIT_IN_MILLIS); // debounce
        ctx.onButton2();
    }
    if (digitalRead(button3Pin) == LOW) {
        delay(Constants::DEBOUNCE_WAIT_IN_MILLIS); // debounce
        ctx.onButton3();
    }
    delay(Constants::LOOP_END_DELAY);
    #ifdef DebugMode
    Serial.println("Current State: " + String(ctx.getState()->getType()));
    #endif
}


