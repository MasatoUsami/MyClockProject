#include "clock.h"

#include "config.h"

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


//==================================================
// Initialize
//==================================================

void clockInit()
{
    currentHour = 12;
    currentMinute = 0;

    currentStep = 0.0f;
}


//==================================================
// Set Home
//==================================================

void clockSetHome()
{
    currentHour = 12;
    currentMinute = 0;

    currentStep = 0.0f;
}


//==================================================
// Get current hour
//==================================================

int clockGetHour()
{
    return currentHour;
}


//==================================================
// Get current minute
//==================================================

int clockGetMinute()
{
    return currentMinute;
}


//==================================================
// Get total minutes
//==================================================

int clockGetTotalMinutes()
{
    return currentHour * 60 + currentMinute;
}


//==================================================
// Get current step
//==================================================

float clockGetStep()
{
    return currentStep;
}


//==================================================
// Calculate steps to target
//==================================================

float clockCalculateSteps(int targetHour, int targetMinute)
{
    int currentTotal =
        currentHour * 60 + currentMinute;

    int targetTotal =
        targetHour * 60 + targetMinute;

    int diff =
        targetTotal - currentTotal;

    // 前進のみ。
    // 目標が現在時刻より前なら12時間後とする。
    if(diff < 0)
    {
        diff += 12 * 60;
    }

    float stepsPerMinute =
        CLOCK_STEPS_PER_REV / 720.0f;

    return diff * stepsPerMinute;
}