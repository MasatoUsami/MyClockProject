#include "config.h"
#include "motor.h"
#include "sensor.h"
#include "state.h"

void setup()
{
    Serial.begin(115200);

    motorInit();
    sensorInit();

    stateInit();
}

void loop()
{
    sensorUpdate();
    stateUpdate();
}