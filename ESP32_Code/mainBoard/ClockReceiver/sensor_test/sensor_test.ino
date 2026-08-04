#include "config.h"
#include "sensor.h"

bool homed = false;

void setup() {

  Serial.begin(115200);

  DEBUG_PRINTLN("");
  DEBUG_PRINTLN("=======================");
  DEBUG_PRINTLN(" Sensor Test Ver1.0");
  DEBUG_PRINTLN("=======================");

  sensorInit();
}

void loop() {

  sensorUpdate();

  if (sensorRisingEdge()) {
    DEBUG_PRINTLN("Magnet Detect");
  }

  if (sensorFallingEdge()) {
    DEBUG_PRINTLN("Magnet Leave");
  }

  if (!homed) {
    if (sensorHomePosition()) {
      DEBUG_PRINTLN("HOME FOUND");
      homed = true;
    }
  }

}
