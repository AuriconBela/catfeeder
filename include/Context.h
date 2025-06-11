#ifndef CONTEXT_H
#define CONTEXT_H

#include "State.h" // Ensure State is defined before use

class Context {
public:
    Context();
    ~Context(); // Destructor declaration
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

Context::~Context() {
    if (currentState) delete currentState;
}
