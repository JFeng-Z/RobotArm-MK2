#include "include.h"
#include "webUI.h"

xMenu menu;
extern int test;

void setup()
{
    Serial.begin(115200);
    buttonInit();
    espUI_Init();
    Menu_Init(&menu);
    ArmInit();
}

void loop()
{
    test = rand();
    Menu_Loop(&menu);
}
