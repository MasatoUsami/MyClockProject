#include "config.h"
#include "sensor.h"

// 現在の状態
static bool currentState = false;

// 前回状態
static bool previousState = false;

// エッジ検出
static bool risingEdge = false;
static bool fallingEdge = false;

// 初期化
void sensorInit()
{
    pinMode(SENSOR_PIN, INPUT_PULLUP);

    currentState = digitalRead(SENSOR_PIN);

    previousState = currentState;
}

// 更新（loop毎に呼ぶ）
void sensorUpdate()
{

    previousState = currentState;

    currentState = digitalRead(SENSOR_PIN);

    risingEdge = false;
    fallingEdge = false;

    if(previousState == HIGH &&
       currentState == LOW)
    {
        risingEdge = true;
    }

    if(previousState == LOW &&
       currentState == HIGH)
    {
        fallingEdge = true;
    }

}

// 現在の状態
bool sensorDetected()
{
    return currentState == LOW;
}

// ONになった瞬間
bool sensorRisingEdge()
{
    return risingEdge;
}

// OFFになった瞬間
bool sensorFallingEdge()
{
    return fallingEdge;
}

