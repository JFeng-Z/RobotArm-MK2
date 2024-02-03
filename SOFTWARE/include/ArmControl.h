#ifndef _ArmControl_H_
#define _ArmControl_H_

#include "include.h"
#include "menu.h"
#include "MenuConfig.h"

void ArmInit(void);
void SetBaseAngle(uint8_t Angle);
void SetBigArmAngle(uint8_t Angle);
void SetForearmAngle(uint8_t Angle);
void SetClampAngle(uint8_t Angle);
void ARM_Control(int x,int y,int z);
double TripleDiff(double Now, double Targrt,double AllTime,double NowTime);
void AngleMove(uint8_t BaseTargrt, uint8_t BigArmTargrt, uint8_t ForearmTargrt, uint8_t ClampTarget);
void ArmMotionInit(void);
void Arm_SetBaseAngle(xpItem item);
void Arm_SetBigArmAngle(xpItem item);
void Arm_SetForearmAngle(xpItem item);
void Arm_ClampAngle(xpItem item);
void ArmMotionSave(xpItem item);
void Move(xpItem item);
void MotionRun(xpItem item);
void ResetAngle(xpItem item);
void Execute_Motions(xpItem item);
void Arm_SetX(xpItem item);
void Arm_SetY(xpItem item);
void Arm_SetZ(xpItem item);
void ArmAxis_Move(xpItem item);

#endif