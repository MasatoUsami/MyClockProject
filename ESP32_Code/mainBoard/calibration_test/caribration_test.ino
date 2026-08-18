#include <Arduino.h>

#include "config.h"
#include "motor.h"
#include "sensor.h"

#define CALIBRATION_STEPS 1055
#define STEP_DELAY_MS 100
#define HOME_DELAY_MS 125

bool doHoming()
{
    Serial.println("Homing start...");

    //========================================
    // すでにセンサー上なら抜ける
    //========================================

    if (sensorDetected())
    {
        Serial.println("Sensor already active.");
        Serial.println("Escaping...");

        while (sensorDetected())
        {
            sensorUpdate();

            motorStepForward();

            delay(125);
        }
    }

    //========================================
    // ホームを探す
    //========================================

    Serial.println("Searching home...");

    while (!sensorHomePosition())
    {
        sensorUpdate();

        motorStepForward();

        delay(125);
    }

    motorDisable();

    Serial.println("HOME POSITION");
    Serial.println("12:00 = 0 degree");

    return true;
}

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("==============================");
    Serial.println("Clock Calibration Test");
    Serial.println("==============================");

    motorInit();
    sensorInit();

    delay(1000);

    Serial.println("Start Homing...");
}

void loop()
{
    static bool completed = false;

    if (completed)
        return;

    //========================================
    // Homing
    //========================================

    if (!doHoming())
        return;

    delay(1000);

    //========================================
    // Calibration movement
    //========================================

    Serial.println("Start calibration...");
    Serial.println("5 revolutions");

    Serial.print("Steps = ");
    Serial.println(CALIBRATION_STEPS);

    for (int i = 0; i < CALIBRATION_STEPS; i++)
    {
        motorStepForward();

        delay(STEP_DELAY_MS);
    }

    Serial.println();
    Serial.println("==============================");
    Serial.println("CALIBRATION COMPLETE");
    Serial.print("Total Steps = ");
    Serial.println(CALIBRATION_STEPS);
    Serial.println("Measure the actual hand position now.");
    Serial.println("==============================");

    // 測定終了まで励磁ON
    completed = true;
}ESP32_Code/mainBoard/calibration_test/caribration_test/caribration_test.ino