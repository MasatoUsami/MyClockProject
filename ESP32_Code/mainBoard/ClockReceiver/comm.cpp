#include "comm.h"

#include <Arduino.h>

#include "config.h"
#include "state.h"

HardwareSerial RS485(2);

//==================================================
// Initialize
//==================================================

void commInit()
{
    RS485.begin(
        RS485_BAUD,
        SERIAL_8N1,
        RS485_RX,
        RS485_TX);

    DEBUG_PRINTLN("RS485 INIT");
}

//==================================================
// Update
//==================================================

void commUpdate()
{
    // DEBUG_PRINTLN("COMM UPDATE");

    if (!RS485.available())
        return;

    DEBUG_PRINTLN("===== RS485 DATA RECEIVED =====");

    int count = RS485.available();

    DEBUG_PRINT("Available bytes = ");
    DEBUG_PRINTLN(count);

    String s = RS485.readStringUntil('\n');

    DEBUG_PRINT("RAW DATA = [");
    DEBUG_PRINT(s);
    DEBUG_PRINTLN("]");

    s.trim();

    DEBUG_PRINT("RS485 RX: ");
    DEBUG_PRINTLN(s);

    int hour;
    int minute;
    int direction;

    int result = sscanf(
        s.c_str(),
        "%d,%d,%d",
        &hour,
        &minute,
        &direction);

    DEBUG_PRINT("sscanf result = ");
    DEBUG_PRINTLN(result);

    if (result != 3)
    {
        DEBUG_PRINTLN("Invalid command format");
        return;
    }

    DEBUG_PRINT("Hour = ");
    DEBUG_PRINTLN(hour);

    DEBUG_PRINT("Minute = ");
    DEBUG_PRINTLN(minute);

    DEBUG_PRINT("Direction = ");
    DEBUG_PRINTLN(direction);

    //==================================================
    // Forward JUMP only
    //==================================================

    if (direction == 0 || direction == 1) {
        stateRequestJump(hour, minute, direction);
    }
    else
    {
        DEBUG_PRINTLN("Invalid direction");
    }   
    
}