#ifndef STATE_H
#define STATE_H

class Context;

class State {
public:
    virtual void enter(Context* ctx) {}
    virtual void update(Context* ctx) {}
    virtual void onButton1(Context* ctx) {}
    virtual void onButton2(Context* ctx) {}
    virtual void onButton3(Context* ctx) {}
    virtual ~State() {}
};

#endif // STATE_H
