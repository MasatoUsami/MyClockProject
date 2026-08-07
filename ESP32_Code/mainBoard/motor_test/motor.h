#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

// 初期化
void motorInit();

// モーター励磁ON
void motorEnable();

// モーター励磁OFF
void motorDisable();

// 正転1ステップ
void motorStepForward();

// 逆転1ステップ
void motorStepReverse();

#endif