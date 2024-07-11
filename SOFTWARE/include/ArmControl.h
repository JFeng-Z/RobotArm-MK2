#ifndef _ArmControl_H_
#define _ArmControl_H_

#include "include.h"

void ArmInit(void);
void SetBaseAngle(void *Angle);
void SetBigArmAngle(void *Angle);
void SetForearmAngle(void *Angle);
void SetClampAngle(void *Angle);
void ARM_Control(int x,int y,int z);
int TripleDiff(double Now, double Targrt,double AllTime,double NowTime);
void AngleMove(uint8_t BaseTargrt, uint8_t BigArmTargrt, uint8_t ForearmTargrt, uint8_t ClampTarget);
void ArmMotionInit(void);
void MotionSave(xpItem AngleItem, uint8_t BaseAng, uint8_t BigArmAng, uint8_t ForearmAng, uint8_t ClampAng);
void ResetAngle(xpMenu Menu);

#endif