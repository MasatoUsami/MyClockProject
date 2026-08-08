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

void loop()
{
    sensorUpdate();
    stateUpdate();
}