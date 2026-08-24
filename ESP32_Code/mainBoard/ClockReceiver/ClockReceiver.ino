#include "config.h"
#include "motor.h"
#include "sensor.h"
#include "state.h"


void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("==========================");
    Serial.println(VERSION);
    Serial.println("==========================");

    motorInit();
    sensorInit();
    stateInit();
}


static bool jumpTestRequested = false;

void loop()
{
    sensorUpdate();

    stateUpdate();

    if (getState() == STATE_RUN && !jumpTestRequested)
    {
        jumpTestRequested = true;

        DEBUG_PRINTLN("=== JUMP REQUEST TEST ===");

        stateRequestJump(12, 30);
    }
}