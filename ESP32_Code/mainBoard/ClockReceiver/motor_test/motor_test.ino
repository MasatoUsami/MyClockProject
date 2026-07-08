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

    for(int i = 1; i <= 200; i++) {
      motorStepForward();

        DEBUG_PRINTF("Step = %d\n", 1);

      delay(20);
    }

    Serial.println("Forward Complete");
    delay(1000);

    Serial.println("Reverse");

    for(int i = 1; i <= 200; i--) {
      motorStepReverse();

      DEBUG_PRINTF("Step = %d\n", 1);
  

      delay(20);
    }

    Serial.println("Reverse Complete");
    delay(1000);
}
