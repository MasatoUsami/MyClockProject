#include "state.h"

#include "config.h"
#include "motor.h"
#include "sensor.h"
#include "clock.h"

//==================================================
// System State
//==================================================
static SystemState state = STATE_INIT;
static SystemState previousState = STATE_INIT;

//==================================================
// Homing State
//==================================================
enum HomingState
{
    HOME_ESCAPE,
    HOME_SEARCH,
    HOME_DONE
};

static HomingState homeState = HOME_ESCAPE;

static bool processHoming();

// Initialize the state machine
void stateInit()
{
    state = STATE_INIT;
    previousState = STATE_INIT;

    homeState = HOME_ESCAPE;
}

// 状態取得
SystemState getState()
{
    return state;
}

// Update the state machine
void stateUpdate()
{
    //==================================================
    // State transition debug
    //==================================================

    if (state != previousState)
    {
        DEBUG_PRINT("STATE -> ");

        switch (state)
        {
        case STATE_INIT:
            DEBUG_PRINTLN("INIT");
            break;

        case STATE_HOMING:
            DEBUG_PRINTLN("HOMING");
            break;

        case STATE_RUN:
            DEBUG_PRINTLN("RUN");
            break;

        case STATE_JUMP:
            DEBUG_PRINTLN("JUMP");
            break;

        case STATE_ERROR:
            DEBUG_PRINTLN("ERROR");
            break;
        }

        previousState = state;
    }

    //==================================================
    // State Machine
    //==================================================

    switch (state)
    {
    case STATE_INIT:
    {
        state = STATE_HOMING;

        break;
    }

    case STATE_HOMING:
    {
        if (processHoming())
        {
            DEBUG_PRINTLN("HOME COMPLETE");

            state = STATE_RUN;
        }

        break;
    }

    case STATE_RUN:
    {
        // 通常時計

        break;
    }

    case STATE_JUMP:
    {
        // JUMP

        break;
    }

    case STATE_ERROR:
    {
        // エラー停止
        motorDisable();

        break;
    }
    }
}

static bool processHoming()
{
    static unsigned long homingStartTime = 0;
    static unsigned long lastStepTime = 0;
    static bool started = false;

    const unsigned long now = millis();

    if (!started)
    {
        started = true;
        homingStartTime = now;
        lastStepTime = now;

        // 既にホームセンサー上なら、先にセンサー外まで抜ける。
        homeState = sensorDetected() ? HOME_ESCAPE : HOME_SEARCH;
    }

    if (now - homingStartTime >= HOME_TIMEOUT)
    {
        DEBUG_PRINTLN("HOME TIMEOUT");
        DEBUG_PRINT("Homing elapsed = ");
        DEBUG_PRINTLN(now - homingStartTime);

        motorDisable();
        state = STATE_ERROR;
        started = false;
        homeState = HOME_ESCAPE;
        return false;
    }

    switch (homeState)
    {
    case HOME_ESCAPE:
    {
        DEBUG_PRINTLN("HOME_ESCAPE");

        if (!sensorDetected())
        {
            homeState = HOME_SEARCH;
            lastStepTime = now;
            break;
        }

        if (now - lastStepTime >= (unsigned long)(1000.0f / HOME_FAST_SPEED))
        {
            motorStepForward();
            lastStepTime = now;
        }
        break;
    }

    case HOME_SEARCH:
    {
        DEBUG_PRINTLN("HOME_SEARCH");

        if (sensorHomePosition())
        {
            motorDisable();

            clockSetHome();

            homeState = HOME_DONE;
            break;
        }

        if (now - lastStepTime >= (unsigned long)(1000.0f / HOME_SLOW_SPEED))
        {
            motorStepForward();
            lastStepTime = now;
        }
        break;
    }

    case HOME_DONE:
    {
        DEBUG_PRINTLN("HOME_DONE");

        // 次回 stateInit() 後に再ホーミングできるよう内部状態を戻す。
        started = false;
        homeState = HOME_ESCAPE;
        return true;
    }
    }

    return false;
}
