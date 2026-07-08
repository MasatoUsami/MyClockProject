#include "config.h"

void setup() {

  Serial.begin(115200);

  Serial.println(VERSION);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(SENSOR_PIN, INPUT_PULLUP);

}

void loop() {

}