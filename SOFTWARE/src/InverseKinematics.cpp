#include "InverseKinematics.h"

int BaseAngle(int x,int y)
{
    if (x >= 0)
    {
        return (PI-(atan(tan(y/x))))*(180/PI);
    }
    return (atan(tan(y/(-x))))*(180/PI);
}

float VerProjection(int x,int y)
{
    return sqrt(x*x+y*y);
}

int BigArmAngle(int x,int y,int z)
{
    int ShortSide;
    float Bias,Ver_Projection=VerProjection(x,y),Angle1,Angle2;
    if (z<Base_High)
    {
        ShortSide=Base_High-z;
        Bias=sqrt(ShortSide*ShortSide+Ver_Projection*Ver_Projection);
        Angle2=atan(Ver_Projection/ShortSide)*(180/PI);
        Angle1=acos((BigArm_Len*BigArm_Len+Bias*Bias-Forearm_Len*Forearm_Len)/(2*BigArm_Len*Bias))*(180/PI);
        return Angle1+Angle2-90;
    }
    else if (z>Base_High)
    {
        ShortSide=z-Base_High;
        Bias=sqrt(ShortSide*ShortSide+Ver_Projection*Ver_Projection);
        Angle2=atan(ShortSide/Ver_Projection)*(180/PI);
        Angle1=acos((BigArm_Len*BigArm_Len+Bias*Bias-Forearm_Len*Forearm_Len)/(2*BigArm_Len*Bias))*(180/PI);
        return Angle1+Angle2;
    }
    Bias=Ver_Projection;
    Angle1=acos((BigArm_Len*BigArm_Len+Bias*Bias-Forearm_Len*Forearm_Len)/(2*BigArm_Len*Bias))*(180/PI);
    return Angle1;
}

int ForearmAngle(int x,int y,int z)
{
    int ShortSide;
    float Bias,Ver_Projection=VerProjection(x,y);
    if (z<Base_High)
    {
        ShortSide=Base_High-z;
        Bias=sqrt(ShortSide*ShortSide+Ver_Projection*Ver_Projection);
    }
    else if (z>Base_High)
    {
        ShortSide=z-Base_High;
        Bias=sqrt(ShortSide*ShortSide+Ver_Projection*Ver_Projection);
    }
    else if (z==Base_High)
    {
        Bias=Ver_Projection;
    }
    return acos((BigArm_Len*BigArm_Len+Forearm_Len*Forearm_Len-Bias*Bias)/(2*BigArm_Len*Forearm_Len))*(180/PI)+BigArmAngle(x,y,z)-90;
}
