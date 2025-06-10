#ifndef CONTEXT_H
#define CONTEXT_H

#include "State.h"

class Context {
public:
    Context();
    void setState(State* state);
    void update();
    void onButton1();
    void onButton2();
    void onButton3();
    State* getState();
    // Add hardware references here (servo, rtc, lcd, etc.)
    // ...
private:
    State* currentState;
};

#endif // CONTEXT_H
