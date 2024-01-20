#ifndef _ArmControl_H_
#define _ArmControl_H_

#include "Arduino.h"
#include <Servo.h> 
#include "app.h"
#include "menu.h"
extern "C"{
#include "InverseKinematics.h"
} 

#define Forearm_U9 13
#define Base_U8 12
#define BigArm_U7 14
#define Clamp_U10 15

typedef struct ArmMotion *pArmMotion;
typedef struct ArmMotion
{
    uint8_t id;
    uint8_t base_angle;
    uint8_t bigArm_angle;
    uint8_t forearm_angle;
    pArmMotion next_motion;
}xArmMotion;

void ArmInit(void);
void SetBaseAngle(uint8_t Angle);
void SetBigArmAngle(uint8_t Angle);
void SetForearmAngle(uint8_t Angle);
void ARM_Control(int x,int y,int z);
double TripleDiff(double Now, double Targrt,double AllTime,double NowTime);
void AngleMove(uint8_t BaseTargrt, uint8_t BigArmTargrt ,uint8_t ForearmTargrt);
void ArmMotionInit(void);
uint8_t MotionSave(uint8_t BaseAng, uint8_t BigArmAng ,uint8_t ForearmAng);
void Execute_Motions(void);

#endif