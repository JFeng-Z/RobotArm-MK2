#include "ArmControl.h"

Servo Forearm,Base,BigArm,Clamp;  

void ArmInit(void)
{
    Forearm.attach(Forearm_U9);        
    Base.attach(Base_U8);      
    BigArm.attach(BigArm_U7);   
    Clamp.attach(Clamp_U10);      

    Forearm.write(0);
    Base.write(0);
    BigArm.write(0);
    Clamp.write(0);
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

double timestep = 3;
double tf = 100;

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

void AngleMove(uint8_t BaseTargrt, uint8_t BigArmTargrt ,uint8_t ForearmTargrt)
{
    uint8_t base = Base.read(), bigArmTargrt = BigArm.read(),forearm = Forearm.read();
    for (double t = 0; t <= tf; t +=timestep) 
    { 
        SetBaseAngle(TripleDiff(base,BaseTargrt,tf,t));
        SetBigArmAngle(TripleDiff(bigArmTargrt,BigArmTargrt,tf,t));
        SetForearmAngle(TripleDiff(forearm,ForearmTargrt,tf,t));
    }
}

pArmMotion pm,head,tail; 

void ArmMotionInit(void)
{
    pm = (pArmMotion)malloc(sizeof(xArmMotion));
    head = pm;
    tail = pm;
    head->next_motion = NULL;
}

void MotionSave(uint8_t BaseAng, uint8_t BigArmAng ,uint8_t ForearmAng)
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
    tail->next_motion = NULL;
}

void Execute_Motions(void)
{
    pArmMotion current_motion = head; // 从第一个节点开始
    while (1)
    {
        while (current_motion != NULL) {
            // 执行当前节点中保存的动作
            AngleMove(current_motion->base_angle, current_motion->bigArm_angle, current_motion->forearm_angle);
            if(Quit_Inspect() == true)return ;
            current_motion = current_motion->next_motion; // 移到下一个节点
        }
        current_motion = head;
    }
}