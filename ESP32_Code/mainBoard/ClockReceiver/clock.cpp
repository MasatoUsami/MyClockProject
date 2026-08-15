#include "clock.h"

#include "config.h"

#include "motor.h"


//==================================================
// Clock internal state
//==================================================

// 現在時刻
static int currentHour = 12;
static int currentMinute = 0;

// 現在位置
//
// 12:00 = 0 step
// 12:00 → 12:00 = CLOCK_STEPS_PER_REV
//
static float currentStep = 0.0f;

static unsigned long lastClockStepTime = 0;

static float stepAccumulator = 0.0f;


//==================================================
// Initialize
//==================================================

void clockInit() {
  currentHour = 12;
  currentMinute = 0;

  currentStep = 0.0f;
}


//==================================================
// Set Home
//==================================================

void clockSetHome() {
  currentHour = 12;
  currentMinute = 0;

  currentStep = 0.0f;
}


//==================================================
// Get current hour
//==================================================

int clockGetHour() {
  int totalMinutes = clockGetTotalMinutes();

  int hour = 12 + totalMinutes / 60;

  if (hour > 12)
    hour -= 12;

  return hour;
}


//==================================================
// Get current minute
//==================================================

int clockGetMinute() {
  int totalMinutes = clockGetTotalMinutes();

  return totalMinutes % 60;
}


//==================================================
// Get total minutes
//==================================================

int clockGetTotalMinutes() {
  float totalMinutes =
    currentStep * 720.0f / CLOCK_STEPS_PER_REV;

  int minutes =
    (int)(totalMinutes + 0.5f);

  minutes %= 720;

  return minutes;
}


//==================================================
// Get current step
//==================================================

float clockGetStep() {
  return currentStep;
}


//==================================================
// Calculate steps to target
//==================================================

float clockCalculateSteps(int targetHour, int targetMinute) {
  int currentTotal =
    currentHour * 60 + currentMinute;

  int targetTotal =
    targetHour * 60 + targetMinute;

  int diff =
    targetTotal - currentTotal;

  // 前進のみ。
  // 目標が現在時刻より前なら12時間後とする。
  if (diff < 0) {
    diff += 12 * 60;
  }

  float stepsPerMinute =
    CLOCK_STEPS_PER_REV / 720.0f;

  return diff * stepsPerMinute;
}


//==================================================
// Start normal clock operation
//==================================================

void clockStart() {
  lastClockStepTime = millis();
  stepAccumulator = 0.0f;
}


//==================================================
// Update normal clock operation
//==================================================

void clockUpdate() {
  unsigned long now = millis();

  unsigned long elapsed =
    now - lastClockStepTime;

  if (elapsed == 0)
    return;

  lastClockStepTime = now;

  float stepsPerSecond =
    CLOCK_STEPS_PER_REV / 43200.0f;

  stepAccumulator +=
    elapsed * stepsPerSecond / 1000.0f;

  while (stepAccumulator >= 1.0f) {
    clockStepForward();

    stepAccumulator -= 1.0f;
  }
}


//==================================================
// Step forward
//==================================================
void clockStepForward() {
  motorStepForward();

  currentStep += 1.0f;

  if (currentStep >= CLOCK_STEPS_PER_REV) {
    currentStep -= CLOCK_STEPS_PER_REV;
  }

  // デバッグ出力(確認用)
  DEBUG_PRINT("Clock Step = ");
  DEBUG_PRINTLN(currentStep);

  DEBUG_PRINT("Clock Time = ");

  int hour = clockGetHour();
  int minute = clockGetMinute();

  DEBUG_PRINT(hour);
  DEBUG_PRINT(":");

  if (minute < 10)
    DEBUG_PRINT("0");

  DEBUG_PRINTLN(minute);
}


//=============================================================
// １２時からの分数
//=============================================================

static int timeToElapsedMinutes(
  int hour,
  int minute) {
  int total =
    hour * 60 + minute;

  total -= 12 * 60;

  if (total < 0) {
    total += 720;
  }

  return total;
}


//=======================================================
// 現在位置との差
//=======================================================

float clockCalculateJumpForwardSteps(
  int targetHour,
  int targetMinute) {
  float targetStep =
    clockGetTargetStep(
      targetHour,
      targetMinute);

  float diff =
    targetStep - currentStep;

  if (diff < 0.0f) {
    diff += CLOCK_STEPS_PER_REV;
  }

  return diff;
}


//============================================
// 目標時刻を時計内部のstepに変換
//============================================

float clockGetTargetStep(
  int targetHour,
  int targetMinute) {
  int elapsedMinutes =
    timeToElapsedMinutes(
      targetHour,
      targetMinute);

  // 12:00を0分として扱う
  if (targetHour == 12) {
    elapsedMinutes = targetMinute;
  } else {
    elapsedMinutes =
      (targetHour * 60 + targetMinute)
      - (12 * 60);

    if (elapsedMinutes < 0) {
      elapsedMinutes += 720;
    }
  }

  return elapsedMinutes * CLOCK_STEPS_PER_REV / 720.0f;
}




//========================================================
// 逆進JUMP
//========================================================
float clockCalculateJumpReverseSteps(
  int targetHour,
  int targetMinute) {
  float targetStep =
    clockGetTargetStep(
      targetHour,
      targetMinute);

  float diff =
    currentStep - targetStep;

  if (diff < 0.0f) {
    diff += CLOCK_STEPS_PER_REV;
  }

  return diff;
}

