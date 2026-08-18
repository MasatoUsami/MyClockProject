#include <Arduino.h>

#include "config.h"
#include "motor.h"
#include "sensor.h"

//==================================================
// Calibration Settings
//==================================================

// 1回転あたりの実測基準step
#define CALIBRATION_STEPS_PER_REV 211

// 測定する回転数
#define CALIBRATION_REVOLUTIONS 5

// モーターstep間隔
#define STEP_DELAY_MS 100

// Homing step間隔
#define HOME_DELAY_MS 125


//==================================================
// Homing
//==================================================

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

            delay(HOME_DELAY_MS);
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

        delay(HOME_DELAY_MS);
    }

    motorDisable();

    Serial.println("HOME POSITION");
    Serial.println("12:00 = 0 degree");

    return true;
}


//==================================================
// Setup
//==================================================

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


//==================================================
// Loop
//==================================================

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

    Serial.println();
    Serial.println("==============================");
    Serial.println("Start calibration...");
    Serial.println("1 revolution = 211 steps");
    Serial.println("Total = 5 revolutions");
    Serial.println("==============================");

    //========================================
    // 211 stepごとに停止
    //========================================

    for (int revolution = 1;
         revolution <= CALIBRATION_REVOLUTIONS;
         revolution++)
    {
        Serial.println();
        Serial.print("Revolution ");
        Serial.print(revolution);
        Serial.println(" start");

        // 211 step動かす
        for (int step = 0;
             step < CALIBRATION_STEPS_PER_REV;
             step++)
        {
            motorStepForward();

            delay(STEP_DELAY_MS);
        }

        // 1回転ごとに停止
        motorDisable();

        Serial.println();
        Serial.println("------------------------------");

        Serial.print("Revolution = ");
        Serial.println(revolution);

        Serial.print("Total Steps = ");
        Serial.println(
            revolution * CALIBRATION_STEPS_PER_REV
        );

        Serial.println("Measure hand position now.");

        Serial.println("------------------------------");

        //====================================
        // 次の1回転まで待つ
        //====================================

        if (revolution < CALIBRATION_REVOLUTIONS)
        {
            Serial.println("Press RESET to repeat from HOME.");
            
            // 次の回転を自動開始
            delay(2000);
        }
    }

    //========================================
    // Complete
    //========================================

    Serial.println();
    Serial.println("==============================");
    Serial.println("CALIBRATION COMPLETE");
    Serial.println("==============================");

    Serial.println("Measured positions:");
    Serial.println("Rev 1 = 211 steps");
    Serial.println("Rev 2 = 422 steps");
    Serial.println("Rev 3 = 633 steps");
    Serial.println("Rev 4 = 844 steps");
    Serial.println("Rev 5 = 1055 steps");

    Serial.println("==============================");

    // 測定終了後は励磁OFF
    motorDisable();

    completed = true;
}