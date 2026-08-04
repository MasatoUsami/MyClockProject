#include "config.h"
#include "sensor.h"


// 現在の状態
static bool currentState = HIGH;

// 前回状態
static bool previousState = HIGH;

// エッジ検出
static bool risingEdge = false;
static bool fallingEdge = false;

// デバウンス用
static bool lastRawState = HIGH;
static unsigned long lastChangeTime = 0;

const unsigned long debounceTime = 10;   // 10ms

// 初期化
void sensorInit()
{
    pinMode(SENSOR_PIN, INPUT);

    currentState = digitalRead(SENSOR_PIN);
    previousState = currentState;

    lastRawState = currentState;
    lastChangeTime = millis();

    risingEdge = false;
    fallingEdge = false;
}

// 更新（loop毎に呼ぶ）
void sensorUpdate() {
    // 毎回クリア
    risingEdge = false;
    fallingEdge = false;

    bool raw = digitalRead(SENSOR_PIN);

    // 状態が変わったら時間を記録
    if (raw != lastRawState) {
        lastRawState = raw;
        lastChangeTime = millis();
    }

    // 一定時間変化しなければ確定
    if (millis() - lastChangeTime >= debounceTime) {
        if (raw != currentState) {
            previousState = currentState;
            currentState = raw;

            if (previousState == HIGH && currentState == LOW) {
                risingEdge = true;
            }

            if (previousState == LOW && currentState == HIGH) {
                fallingEdge = true;
            }
        }
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

// ホームポジション
bool sensorHomePosition() {
    return sensorRisingEdge();
}
