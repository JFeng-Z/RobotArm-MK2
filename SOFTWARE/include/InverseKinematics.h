#ifndef _InverseKinematics_H_
#define _InverseKinematics_H_

#include "Arduino.h"

#define Base_High 90
#define BigArm_Len 135
#define Forearm_Len 146

int BaseAngle(int x,int y);
int BigArmAngle(int x,int y,int z);
int ForearmAngle(int x,int y,int z);

#endif