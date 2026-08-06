#include "state.h"

#include "motor.h"
#include "sensor.h"

static SystemState state;

// Initialize the state machine
void stateInit()
{
    state = STATE_HOMING;
}

// 状態取得
SystemState getState()
{
    return state;
}

// Update the state machine
void stateUpdate()
{

    switch(state)
    {

        case STATE_HOMING:

            // 後で実装

            break;

        case STATE_RUN:

            // 後で実装

            break;

        case STATE_JUMP:

            // 後で実装

            break;

        case STATE_ERROR:

            // 後で実装

            break;
    }

}
