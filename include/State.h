#ifndef STATE_H
#define STATE_H

class Context;

class State {
public:
    enum StateType { NORMAL, ROLLUP, OPEN, ROLLDOWN, HOUR_SET, MINUTE_SET, PROXIMITYSTATE, IDLE };
    virtual void enter(Context* ctx) {}
    virtual void update(Context* ctx) {}
    virtual void onButton1(Context* ctx) {}
    virtual void onButton2(Context* ctx) {}
    virtual void onButton3(Context* ctx) {}
    virtual StateType getType() const = 0;
    virtual unsigned long getLoopDelay() const = 0; // Új függvény
    virtual ~State() {}
};

#endif // STATE_H
