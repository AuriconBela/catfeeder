#ifndef STATE_H
#define STATE_H

class Context;

class State {
public:
    enum StateType { NORMAL, ROLLDOWN, PROXIMITY, 
                     ROLLUP, OPEN, HOUR_SET, MINUTE_SET };
    virtual void enter(Context* ctx) {}
    virtual void update(Context* ctx) {}
    virtual void onButton1(Context* ctx) {}
    virtual void onButton2(Context* ctx) {}
    virtual void onButton3(Context* ctx) {}
    virtual StateType getType() const = 0;
    virtual ~State() {}
};

#endif // STATE_H
