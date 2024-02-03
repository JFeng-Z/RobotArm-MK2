#ifndef _ARMROBOTCONFIG_H_
#define _ARMROBOTCONFIG_H_

#define Forearm_U9 13
#define Base_U8 12
#define BigArm_U7 14
#define Clamp_U10 15

typedef struct armRobotangle
{
    int Base_Angle;
    int BigArm_Angle;
    int Forearm_Angle;
    int Clamp_Angle;
    int AxisX;
    int AxisY;
    int AxisZ;
}ArmRobot_Angle;

#endif