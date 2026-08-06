#ifndef STATE_H
#define STATE_H

enum SystemState
{
    STATE_HOMING,
    STATE_RUN,
    STATE_JUMP,
    STATE_ERROR
};

void stateInit();
void stateUpdate();

SystemState getState();

#endif