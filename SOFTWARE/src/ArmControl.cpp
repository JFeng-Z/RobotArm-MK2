#include "ArmControl.h"
#include <Servo.h> 
#include "ArmRobotConfig.h"
#include "disp_dirver.h"

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

void SetBaseAngle(uint8_t Angle)
{
    Base.write(Angle);
}

void SetBigArmAngle(uint8_t Angle)
{
    BigArm.write(Angle);
}

void SetForearmAngle(uint8_t Angle)
{
    Forearm.write(Angle);
}

void SetClampAngle(uint8_t Angle)
{
    Clamp.write(Angle);
}

double TripleDiff(double Now, double Targrt,double AllTime,double NowTime)
{
    static double a0;
    static double a1;
    static double a2;
    static double a3;

    a0 = Now;
    a1 = 0;
    a2 = 3/(pow(AllTime,2))*(Targrt - Now);
    a3 = -2/(pow(AllTime,3))*(Targrt - Now);
    return (a0 + a1*(NowTime) +a2*(pow(NowTime,2)) +a3*(pow(NowTime,3))); 
}

double timestep = 3;
double tf = 100;

void ARM_Control(int x,int y,int z)
{
    uint8_t base = Base.read(), bigArm = BigArm.read(), forearm = Forearm.read();
    for (double t = 0; t <= tf; t +=timestep) 
    { 
        Quit_Inspect();
        SetBaseAngle(TripleDiff(base, BaseAngle(x,y), tf, t));
        SetBigArmAngle(TripleDiff(bigArm, BigArmAngle(x,y,z), tf, t));
        SetForearmAngle(TripleDiff(forearm, ForearmAngle(x,y,z), tf, t));
    }
}

void AngleMove(uint8_t BaseTargrt, uint8_t BigArmTargrt, uint8_t ForearmTargrt, uint8_t ClampTarget)
{
    uint8_t base = Base.read(), bigArmTargrt = BigArm.read(), forearm = Forearm.read(), clamp = Clamp.read();
    for (double t = 0; t <= tf; t +=timestep) 
    { 
        SetBaseAngle(TripleDiff(base,BaseTargrt,tf,t));
        SetBigArmAngle(TripleDiff(bigArmTargrt,BigArmTargrt,tf,t));
        SetForearmAngle(TripleDiff(forearm,ForearmTargrt,tf,t));
        SetClampAngle(TripleDiff(clamp,ClampTarget,tf,t));
    }
}

pArmMotion pm,head,tail; 
xItem angleItem;
extern xPage MotionTree_Page;

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
    angleItem.SwitchState = true;
    AddItem("Init motion", SWITCH, NULL, &angleItem, &MotionTree_Page, NULL, Move);
    head->angle_item = &angleItem;
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
    tail->next_motion = NULL;
    tail->angle_item = AngleItem;
}
