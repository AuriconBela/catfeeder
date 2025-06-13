#ifndef PROXIMITY_TRANSITION_MANAGER_H
#define PROXIMITY_TRANSITION_MANAGER_H

#include "State.h"

class ProximityTransitionManager {
  public:
    enum Transition {
      NONE,
      TO_PROXIMITY,
      TO_NORMAL
    };
    void updateBuffer(bool proximity);
    Transition checkTransition(bool lastProximity, State* currentState);
  private:
    bool buffer[3] = {false, false, false};
    int index = 0;
};

#endif // PROXIMITY_TRANSITION_MANAGER_H
