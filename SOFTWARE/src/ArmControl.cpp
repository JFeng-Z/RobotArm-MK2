#include "ArmControl.h"
#include <Servo.h> 
#include "ArmRobotConfig.h"
#include "disp_dirver.h"
#include "image.h"

Servo Forearm, Base, BigArm, Clamp;  

void ArmInit(void)
{
    Forearm.attach(Forearm_U9);        
    Base.attach(Base_U8);      
    BigArm.attach(BigArm_U7);   
    Clamp.attach(Clamp_U10);      

    Forearm.write(0);
    Base.write(0);
    BigArm.write(0);
    Clamp.write(180);

    ArmMotionInit();
}

void SetBaseAngle(void *Angle)
{
    Base.write(*(int *)Angle);
}

void SetBigArmAngle(void *Angle)
{
    BigArm.write(*(int *)Angle);
}

void SetForearmAngle(void *Angle)
{
    Forearm.write(*(int *)Angle);
}

void SetClampAngle(void *Angle)
{
    Clamp.write(*(int *)Angle);
}

int TripleDiff(double Now, double Targrt,double AllTime,double NowTime)
{
    static double a0;
    static double a1;
    static double a2;
    static double a3;

    a0 = Now;
    a1 = 0;
    a2 = 3/(pow(AllTime,2))*(Targrt - Now);
    a3 = -2/(pow(AllTime,3))*(Targrt - Now);
    return (int)(a0 + a1*(NowTime) +a2*(pow(NowTime,2)) +a3*(pow(NowTime,3))); 
}

double timestep = 3;
double tf = 100;

void ARM_Control(int x,int y,int z)
{
    uint8_t base = Base.read(), bigArm = BigArm.read(), forearm = Forearm.read();
    int base_diff, bigArm_diff, forearm_diff, clamp_diff;
    for (double t = 0; t <= tf; t +=timestep) 
    { 
        base_diff = TripleDiff(base, BaseAngle(x,y), tf, t);
        bigArm_diff = TripleDiff(bigArm, BigArmAngle(x,y,z), tf, t);
        forearm_diff = TripleDiff(forearm, ForearmAngle(x,y,z), tf, t);
        SetBaseAngle(&base_diff);
        SetBigArmAngle(&bigArm_diff);
        SetForearmAngle(&forearm_diff);
    }
}

void AngleMove(uint8_t BaseTargrt, uint8_t BigArmTargrt, uint8_t ForearmTargrt, uint8_t ClampTarget)
{
    uint8_t base = Base.read(), bigArmTargrt = BigArm.read(), forearm = Forearm.read(), clamp = Clamp.read();
    int base_diff, bigArm_diff, forearm_diff, clamp_diff;
    for (double t = 0; t <= tf; t +=timestep) 
    { 
        base_diff = TripleDiff(base, BaseTargrt, tf, t);
        bigArm_diff = TripleDiff(bigArmTargrt, BigArmTargrt, tf, t);
        forearm_diff = TripleDiff(forearm, ForearmTargrt, tf, t);
        clamp_diff = TripleDiff(clamp, ClampTarget, tf, t);
        SetBaseAngle(&base_diff);
        SetBigArmAngle(&bigArm_diff);
        SetForearmAngle(&forearm_diff);
        SetClampAngle(&clamp_diff);
    }
}

pArmMotion pm,head,tail; 
xItem angleItem;
extern xPage ARM_Control_Page;

void ArmMotionInit(void)
{
    pm = (pArmMotion)malloc(sizeof(xArmMotion));
    head = pm;
    tail = pm;
    head->next_motion = tail;
    head->base_angle = 0;
    head->bigArm_angle = 0;
    head->clamp_angle = 180;
    head->forearm_angle = 0;
    head->angle_item = &angleItem;
    AddItem("Init motion", ONCE_FUNCTION, NULL, logo_allArray[9], &angleItem, &ARM_Control_Page, NULL, Move);
}

void MotionSave(xpItem AngleItem, uint8_t BaseAng, uint8_t BigArmAng, uint8_t ForearmAng, uint8_t ClampAng)
{
    static uint8_t ID;
    ID ++;
    pm = (pArmMotion)malloc(sizeof(ArmMotion));
    tail->next_motion = pm;
    tail = pm;
    tail->id = ID;
    tail->base_angle = BaseAng;
    tail->bigArm_angle = BigArmAng;
    tail->forearm_angle = ForearmAng;
    tail->clamp_angle = ClampAng;
    tail->next_motion = head;
    tail->angle_item = AngleItem;
}

ArmRobot_Angle ArmRobotAngle = {0, 0, 0, 180, 10, 10, 10};

void ResetAngle(xpMenu Menu)
{
    ArmRobotAngle.Base_Angle = 0;
    ArmRobotAngle.BigArm_Angle = 0;
    ArmRobotAngle.Forearm_Angle = 0;
    ArmRobotAngle.Clamp_Angle = 180;
    SetBaseAngle(&ArmRobotAngle.Base_Angle);
    SetBigArmAngle(&ArmRobotAngle.BigArm_Angle);
    SetForearmAngle(&ArmRobotAngle.Forearm_Angle);
    SetClampAngle(&ArmRobotAngle.Clamp_Angle);
}

void kinematics(void *value) 
{
    ARM_Control(ArmRobotAngle.AxisX, ArmRobotAngle.AxisY, ArmRobotAngle.AxisZ);
}