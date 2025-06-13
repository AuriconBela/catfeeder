#include "../include/ProximityTransitionManager.h"

void ProximityTransitionManager::updateBuffer(bool proximity) {
    buffer[index] = proximity;
    index = (index + 1) % 3;
}

ProximityTransitionManager::Transition ProximityTransitionManager::checkTransition(bool lastProximity, State* currentState) {
    bool allProx = buffer[0] && buffer[1] && buffer[2];
    bool noneProx = !buffer[0] && !buffer[1] && !buffer[2];
    if (allProx && !lastProximity) {
        if (currentState->getType() == State::NORMAL || currentState->getType() == State::ROLLDOWN) {
            return TO_PROXIMITY;
        }
    } else if (noneProx && lastProximity) {
        if (currentState->getType() == State::PROXIMITYSTATE) {
            return TO_NORMAL;
        }
    }
    return NONE;
}
