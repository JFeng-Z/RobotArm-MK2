#include "button.h"

//ADC按键检测
BtnState ButtonScan(void)
{
    int Sensor=analogRead(A0);
    float Voltage=Sensor*(3.3/1024.0);
    
    if(Voltage>=1.80&&Voltage<=1.84)return Up;
    else if(Voltage>=2.98&&Voltage<=3.02)return Down;
    else if(Voltage>=1.21&&Voltage<=1.25)return Center;
    else if(Voltage>=0.63&&Voltage<=0.67)return Right;
    else if(Voltage>=2.39&&Voltage<=2.43)return Lift;

    return NONE_PRESS;
}