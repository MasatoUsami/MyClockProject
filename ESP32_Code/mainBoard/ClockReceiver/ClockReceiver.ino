#include "config.h"
#include "motor.h"
#include "sensor.h"
#include "state.h"
#include "comm.h"


void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println("==========================");
  Serial.println(VERSION);
  Serial.println("==========================");

  motorInit();
  sensorInit();
  stateInit();
  commInit();
}


void loop() {
  sensorUpdate();

  commUpdate();

  stateUpdate();
}