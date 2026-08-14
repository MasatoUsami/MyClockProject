#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>

//==================================================
// Clock initialization
//==================================================

void clockInit();

//==================================================
// Home position
//==================================================

// ホーミング完了時に12:00として初期化
void clockSetHome();

//==================================================
// Clock position
//==================================================

// 現在の時
int clockGetHour();

// 現在の分
int clockGetMinute();

// 現在の総分数
int clockGetTotalMinutes();

// 現在のステップ位置
float clockGetStep();

//==================================================
// Target calculation
//==================================================

// 指定時刻までの必要ステップ数
float clockCalculateSteps(int targetHour, int targetMinute);

#endif