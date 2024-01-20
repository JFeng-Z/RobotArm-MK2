#ifndef _app_H
#define _app_H

#include "Arduino.h"
#include <U8g2lib.h>
#include "menu.h"
extern "C"{
#include "button.h"
}
#include "ArmControl.h"

void DialogScaleShow(uint16_t x,uint16_t y,uint16_t w,uint16_t h);
void Quit_Inspect(void);
void AppBreak(void);
void White_Dark_Day(void);
void Show_MPU6050(void);
void Show_GitHub(void);
void Show_Bilibili(void);
void Setting_Speed(void);
void Show_Log(void);
void Arm_SetX(void);
void Arm_SetY(void);
void Arm_SetZ(void);
void ArmAxis_Move(void);
void Arm_SetBaseAngle(void);
void Arm_SetBigArmAngle(void);
void Arm_SetForearmAngle(void);
void ArmMotionInit(void);
void ArmMotionSave(void);
void MotionRun(void);
void ResetAngle(void);

#endif