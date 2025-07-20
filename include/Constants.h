#ifndef CONSTANTS_H
#define CONSTANTS_H

class Constants {
public:
    // Serial communication details
    static constexpr int SERIAL_BAUD_RATE = 9600;
    static constexpr int WAIT_TIME_IN_MILLIS = 10;
    // General settings
    static constexpr unsigned int DEBOUNCE_WAIT_IN_MILLIS = 200;
    static constexpr unsigned int LOOP_END_DELAY = 1000;
    static constexpr unsigned int LOOP_END_DELAY_SHORT = 100;
    static constexpr unsigned int IDLE_STATE_DELAY = 1000;    
    // Pin numbers
    static constexpr int BUTTON1_PIN = 5;
    static constexpr int BUTTON2_PIN = 6;
    static constexpr int SERVO_BUTTON_PIN = 10;    
    static constexpr int BUTTON3_PIN = 7;
    static constexpr int BUTTON4_PIN = 9;
    // static constexpr int VOLTAGE_DIVIDER_PIN = A5;
    // Proximity
    static constexpr int PROXIMITY_THRESHOLD = 150;
    static constexpr int PROXIMITY_BUFFER_SIZE = 3;
    // LCD
    static constexpr int LCD_ADDR = 0x27;
    static constexpr int LCD_COLS = 16;
    static constexpr int LCD_ROWS = 2;
    // Servo
    static constexpr int SERVO_PIN = 8;
    // Debug
    // static constexpr bool DEBUG_MODE = true;
    // static constexpr bool SERVO_DEBUG_MODE = true;
    // String constants
    static constexpr const char* MSG_PROXIMITY_SENSOR_FAIL = "Failed to initialize proximity sensor!";
    static constexpr const char* MSG_PROXIMITY_SENSOR_ENABLED = "Proximity sensor enabled.";
    static constexpr const char* MSG_PROXIMITY_SENSOR_INIT = "APDS-9930 Proximity Sensor Test";
    // Feeding data
    static constexpr int FEED_HOURS[3] = {7, 12, 19};
    static constexpr int ROLLUP_ANGLE = 30;
    static constexpr int ROLLDOWN_ANGLE = 160;
    // static constexpr int ROLLUP_ANGLE = 160;
    // static constexpr int ROLLDOWN_ANGLE = 30;    
    static constexpr int OPEN_INTERVAL_IN_MILLIS = 5000;
    static constexpr int MOVE_INTERVAL_IN_MILLIS = 2000;
    static constexpr int MOVE_STEP_INTERVAL_IN_MILLIS = 20;
    // Voltage divider
    // static constexpr float VOLTAGE_DIVIDER_RESISTOR_1 = 10000.0;
    // static constexpr float VOLTAGE_DIVIDER_RESISTOR_2 = 10000.0;
};

#endif // CONSTANTS_H

#ifndef DEBUG_MODE
//#define DEBUG_MODE
#endif 

#ifndef SERVO_DEBUG_MODE
//#define SERVO_DEBUG_MODE
#endif 


