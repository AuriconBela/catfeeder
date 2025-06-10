#include "Context.h"

Context::Context() : currentState(nullptr) {}

void Context::setState(State* state) {
    if (currentState) delete currentState;
    currentState = state;
    if (currentState) currentState->enter(this);
}

void Context::update() {
    if (currentState) currentState->update(this);
}

void Context::onButton1() {
    if (currentState) currentState->onButton1(this);
}

void Context::onButton2() {
    if (currentState) currentState->onButton2(this);
}

void Context::onButton3() {
    if (currentState) currentState->onButton3(this);
}

State* Context::getState() {
    return currentState;
}
