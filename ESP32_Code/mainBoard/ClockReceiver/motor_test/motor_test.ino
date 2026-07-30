#include "config.h"
#include "motor.h"

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("===========================");
    Serial.println("Motor Test Ver1.0");
    Serial.println("===========================");

    motorInit();
}

void loop()
{
    DEBUG_PRINTLN("Forward");

<<<<<<< HEAD
    for(int i = 1; i <= 200; i++) {
      motorStepForward();

        DEBUG_PRINTF("Step = %d\n", 1);
=======
    for(int i = 1; i <= 218; i++) {
      motorStepForward();

        DEBUG_PRINTF("Step = %3d\n", i);
>>>>>>> 91a6792 (First commit. Clock　Project　全てです。ESP32用コードも含まれています。SwitchBox制御用。MainBoard(Motor駆動用)。RS485通信。MainBoardはモーターテストコードまで。今後ホールセンサーを追加して、通信機能までコードを完成させていく予定です。)

      delay(20);
    }

    Serial.println("Forward Complete");
    delay(1000);

    Serial.println("Reverse");

<<<<<<< HEAD
    for(int i = 1; i <= 200; i--) {
      motorStepReverse();

      DEBUG_PRINTF("Step = %d\n", 1);
=======
    for(int i = 1; i <= 218; i++) {
      motorStepReverse();

      DEBUG_PRINTF("Step = %3d\n", i);
>>>>>>> 91a6792 (First commit. Clock　Project　全てです。ESP32用コードも含まれています。SwitchBox制御用。MainBoard(Motor駆動用)。RS485通信。MainBoardはモーターテストコードまで。今後ホールセンサーを追加して、通信機能までコードを完成させていく予定です。)
  

      delay(20);
    }

    Serial.println("Reverse Complete");
    delay(1000);
}
