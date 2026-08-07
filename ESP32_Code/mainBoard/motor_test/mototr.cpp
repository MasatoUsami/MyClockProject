#include <Arduino.h>

#include "config.h"
#include "motor.h"

//==================================================
// モーター内部変数
//==================================================

// 現在の励磁位置
static int stepIndex = 0;

// モーター有効状態
static bool motorEnabled = false;


//==================================================
// 2相励磁シーケンス
//==================================================

const uint8_t seq[4][4] =
{
    {1,1,0,0},
    {0,1,1,0},
    {0,0,1,1},
    {1,0,0,1}
};

//==================================================
// 内部関数
//==================================================

static void applyStep(int index)
{
    digitalWrite(IN1, seq[index][0]);
    digitalWrite(IN2, seq[index][1]);
    digitalWrite(IN3, seq[index][2]);
    digitalWrite(IN4, seq[index][3]);
}

//==================================================
// 初期化
//==================================================

void motorInit()
{

    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);
    pinMode(IN3,OUTPUT);
    pinMode(IN4,OUTPUT);

    motorDisable();

}

//==================================================
// 励磁ON
//==================================================

void motorEnable()
{
    motorEnabled = true;

    applyStep(stepIndex);
}

//==================================================
// 励磁OFF
//==================================================

void motorDisable()
{
    motorEnabled = false;

    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
}

//==================================================
// 正転1step
//==================================================

void motorStepForward()
{

    if(!motorEnabled)
        motorEnable();

    stepIndex++;

    if(stepIndex>=4)
        stepIndex=0;

    applyStep(stepIndex);

}

//==================================================
// 逆転1step
//==================================================

void motorStepReverse()
{

    if(!motorEnabled)
        motorEnable();

    stepIndex--;

    if(stepIndex<0)
        stepIndex=3;

    applyStep(stepIndex);

}

