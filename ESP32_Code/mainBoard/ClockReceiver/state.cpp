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


//==================================================
// Jump State
//==================================================

static float jumpTargetStep = 0.0f;
static float jumpRemainingStep = 0.0f;

static unsigned long jumpLastStepTime = 0;

static bool jumpStarted = false;


//==================================================
// Function Prototypes
//==================================================

static bool processHoming();
static bool processJump();

static void startJumpForward(
    int targetHour,
    int targetMinute);


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

      // clockStart();
      startJumpForward(12, 10);

      // state = STATE_RUN;
    }

    break;
  }

  case STATE_RUN: // 正常運転
  {
    clockUpdate();

    break;
  }

  case STATE_JUMP:
  {
    if (processJump())
    {
      DEBUG_PRINTLN("JUMP COMPLETE");

      clockStart();

      state = STATE_RUN;
    }

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

static bool processJump()
{
  const unsigned long now = millis();

  if (!jumpStarted)
  {
    return true;
  }

  const unsigned long stepInterval =
      (unsigned long)(1000.0f / JUMP_STEP_PER_SEC);

  if (jumpRemainingStep <= 0.0f)
  {
    jumpStarted = false;

    return true;
  }

  if (now - jumpLastStepTime >= stepInterval)
  {
    clockStepForward();

    jumpRemainingStep -= 1.0f;

    jumpLastStepTime = now;

    DEBUG_PRINT("JUMP Step, Remaining = ");
    DEBUG_PRINTLN(jumpRemainingStep);
  }

  return false;
}

static void startJumpForward(int targetHour, int targetMinute)
{
  jumpTargetStep =
      clockGetTargetStep(targetHour, targetMinute);

  jumpRemainingStep =
      clockCalculateJumpForwardSteps(
          targetHour,
          targetMinute);

  jumpLastStepTime = millis();

  jumpStarted = true;

  DEBUG_PRINTLN("JUMP START");

  DEBUG_PRINT("Target Step = ");
  DEBUG_PRINTLN(jumpTargetStep);

  DEBUG_PRINT("Remaining Step = ");
  DEBUG_PRINTLN(jumpRemainingStep);

  state = STATE_JUMP;
}