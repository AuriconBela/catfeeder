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
#define ServoDebugMode Constants::DEBUG_MODE && Constants::SERVO_DEBUG_MODE

Servo feederServo;
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(Constants::LCD_ADDR, 
                      Constants::LCD_COLS, 
                      Constants::LCD_ROWS);

APDS9930 apds = APDS9930();
Context ctx;
bool lastProximity = false;
bool isProximityEnabled = false;

ProximityTransitionManager proximityManager;
//https://github.com/Depau/APDS9930/issues/7
void setup() {
    #ifdef DebugMode
    Serial.begin(Constants::SERIAL_BAUD_RATE);
    // Wait for Serial to be ready
    while (!Serial) {
        delay(Constants::WAIT_TIME_IN_MILLIS); 
    }    
    #endif      

    pinMode(Constants::BUTTON1_PIN, INPUT_PULLUP);
    pinMode(Constants::BUTTON2_PIN, INPUT_PULLUP);
    pinMode(Constants::BUTTON3_PIN, INPUT_PULLUP);
    pinMode(Constants::BUTTON4_PIN, INPUT_PULLUP);
    pinMode(Constants::SERVO_BUTTON_PIN, INPUT);

    // Initialize I2C communication
    Wire.begin(); 
    // Initialize APDS-9930 (configure I2C and initial values)
    if (apds.init()){
        isProximityEnabled = apds.setProximityGain(PGAIN_2X) && apds.enableProximitySensor(false);
    }
  
    feederServo.attach(Constants::SERVO_PIN);
    feederServo.write(Constants::ROLLUP_ANGLE); // Set initial position
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
    } else {
        #ifdef DebugMode
        Serial.println(Constants::MSG_PROXIMITY_SENSOR_FAIL);
        #endif
    }

    ctx.update();
    if (digitalRead(Constants::BUTTON1_PIN) == LOW) {
        delay(Constants::DEBOUNCE_WAIT_IN_MILLIS); // debounce
        ctx.onButton1();
    }
    if (digitalRead(Constants::BUTTON2_PIN) == LOW) {
        delay(Constants::DEBOUNCE_WAIT_IN_MILLIS); // debounce
        ctx.onButton2();
    }
    if (digitalRead(Constants::BUTTON3_PIN) == LOW) {
        delay(Constants::DEBOUNCE_WAIT_IN_MILLIS); // debounce
        ctx.onButton3();
    }
    delay(Constants::LOOP_END_DELAY);
    #ifdef DebugMode
    Serial.println("Current State: " + String(ctx.getState()->getType()));
    #endif
}


