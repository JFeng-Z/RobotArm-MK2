#include <Arduino.h>         
#include "menu.h"
#include "ArmControl.h"
extern "C"{
#include "InverseKinematics.h"
} 

void setup()
{
    Serial.begin(115200);
    ArmInit();
    Menu_Init();
}

void loop()
{
    Menu_Task();
}
