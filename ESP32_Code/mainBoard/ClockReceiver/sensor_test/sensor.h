#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

// 初期化
void sensorInit();

// 更新（loop毎に呼ぶ）
void sensorUpdate();

// 現在の状態
bool sensorDetected();

// ONになった瞬間
bool sensorRisingEdge();

// OFFになった瞬間
bool sensorFallingEdge();

#endif