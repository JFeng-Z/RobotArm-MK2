#ifndef _ArmControl_H_
#define _ArmControl_H_

#include "include.h"

void ArmInit(void);
void SetBaseAngle(uint8_t Angle);
void SetBigArmAngle(uint8_t Angle);
void SetForearmAngle(uint8_t Angle);
void SetClampAngle(uint8_t Angle);
void ARM_Control(int x,int y,int z);
double TripleDiff(double Now, double Targrt,double AllTime,double NowTime);
void AngleMove(uint8_t BaseTargrt, uint8_t BigArmTargrt, uint8_t ForearmTargrt, uint8_t ClampTarget);
void ArmMotionInit(void);
void MotionSave(xpItem AngleItem, uint8_t BaseAng, uint8_t BigArmAng, uint8_t ForearmAng, uint8_t ClampAng);

#endif