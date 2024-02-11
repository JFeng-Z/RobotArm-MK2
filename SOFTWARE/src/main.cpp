#include "include.h"
#include "webUI.h"

void setup()
{
    Serial.begin(115200);
    buttonInit();
    ArmInit();
    Menu_Init();
    espUI_Init();
}

void loop()
{
    Menu_Task();
}
