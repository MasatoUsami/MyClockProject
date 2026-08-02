#include "config.h"
#include "sensor.h"

void setup()
{

    Serial.begin(115200);

    sensorInit();

}

void loop()
{

    sensorUpdate();

    if(sensorRisingEdge())
    {
        DEBUG_PRINTLN("Magnet Detect");
    }

    if(sensorFallingEdge())
    {
        DEBUG_PRINTLN("Magnet Leave");
    }

    if(sensorHomePosition())
    {
        DEBUG_PRINTLN("Home Position");
    }

}

