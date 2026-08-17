#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

//==================================================
// Version
//==================================================

#define VERSION "Clock Receiver Ver0.11"

//==================================================
// Motor
//==================================================

#define IN1 18
#define IN2 19
#define IN3 21
#define IN4 22

//==================================================
// Hall Sensor
//==================================================

#define SENSOR_PIN 34

#define DEBUG_ENABLE 1

//==================================================
// RS485 (UART2)
//==================================================

#define RS485_RX 16
#define RS485_TX 17

#define RS485_BAUD 9600

//==================================================
// Motor
//==================================================
// モーター本来の仕様（参考値）
#define MOTOR_STEPS_PER_REV 200.0f

//==================================================
// Clock Mechanism
//==================================================

// 時計機構の設計値
#define DESIGN_STEPS_PER_REV 200.0f

// 実機キャリブレーション値
// 組み立て後の実測値
// デフォルトのキャリブレーション値
#define DEFAULT_CLOCK_STEPS_PER_REV 211.0f

//==================================================
// Clock Mechanism
//==================================================

// 分針1回転 = 1時間
#define CLOCK_STEPS_PER_REV 211.0f

// 12時間で分針12回転
#define CLOCK_STEPS_PER_12H (CLOCK_STEPS_PER_REV * 12.0f)

// 通常運転速度
#define NORMAL_STEP_PER_SEC (CLOCK_STEPS_PER_12H / 43200.0f)


// 現在使用する値
#define CLOCK_STEPS_PER_REV DEFAULT_CLOCK_STEPS_PER_REV

#define NORMAL_STEP_PER_SEC (CLOCK_STEPS_PER_REV / 3600.0f)

// JUMP速度 [step/sec]
#define JUMP_STEP_PER_SEC 10.0f

//==================================================
// Clock
//==================================================

#define CLOCK_INTERVAL 60000UL

//==================================================
// Homing
//==================================================

// ホーミング高速
#define HOME_FAST_SPEED 40.0f

// ホーミング低速
#define HOME_SLOW_SPEED 8.0f

// タイムアウト(ms)
#define HOME_TIMEOUT 35000

//==================================================
// Debug
//==================================================

#define DEBUG_ENABLE 1

#define DEBUG_MOTOR 1
#define DEBUG_COMM 1
#define DEBUG_STATE 1
#define DEBUG_SENSOR 1

#if DEBUG_ENABLE
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTF(...)

#endif

#endif