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

//==================================================
// Normal clock operation
//==================================================

// 通常運転開始
void clockStart();

// 通常運転更新
// loop()から繰り返し呼ぶ
void clockUpdate();

// 現在位置を1step進める
// 実際のモーターstepと時計位置を同期
void clockStepForward();

//==================================================
// JUMP
//==================================================

// 前進方向のJUMPに必要なstep数
float clockCalculateJumpForwardSteps(
    int targetHour,
    int targetMinute
);

// 逆進方向のJUMPに必要なstep数
float clockCalculateJumpReverseSteps(
    int targetHour,
    int targetMinute
);
 
//=======================================================
// JUMP 計算結果の確認
//=======================================================

float clockGetTargetStep(
    int targetHour,
    int targetMinute
);

#endif