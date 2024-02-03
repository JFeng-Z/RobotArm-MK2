#include "ArmControl.h"
#include "ArmRobotConfig.h"
#include "disp_dirver.h"

Servo Forearm, Base, BigArm, Clamp;  
ArmRobot_Angle ArmRobotAngle = {0, 0, 0, 180, 0, 0, 0};

typedef struct ArmMotion *pArmMotion;
typedef struct ArmMotion
{
    xpItem angle_item;
    uint8_t id;
    uint8_t base_angle;
    uint8_t bigArm_angle;
    uint8_t forearm_angle;
    uint8_t clamp_angle;
    pArmMotion next_motion;
}xArmMotion;

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

void Arm_SetBaseAngle(xpItem item)
{
    char value[20] = {0};
    DialogScaleShow(0, 12, 127, 32);

    Quit_Inspect();

    if(ButtonScan()==Up && (*item->data) < 180)(*item->data) += 1;
    if(ButtonScan()==Down && (*item->data) > 0)(*item->data) -= 1;
    SetBaseAngle((*item->data));
    sprintf(value, "BaseAngle = %d", (*item->data));
    OLED_DrawStr(8, 25, value);
    Draw_Scrollbar(8, 30, 100, 6, 3, 0, 180, (*item->data));
    OLED_SendBuffer();
}

void Arm_SetBigArmAngle(xpItem item)
{
    char value[20] = {0};
    DialogScaleShow(4,12,120,32);

    Quit_Inspect();

    if(ButtonScan() == Up && (*item->data) < 180)(*item->data) += 1;
    if(ButtonScan() == Down && (*item->data) > 0)(*item->data) -= 1;
    SetBigArmAngle((*item->data));
    sprintf(value, "BigArmAngle = %d", (*item->data));
    OLED_DrawStr(8, 25, value);
    Draw_Scrollbar(8, 30, 100, 6, 3, 0, 180, (*item->data));
    OLED_SendBuffer();
}

void Arm_SetForearmAngle(xpItem item)
{
    char value[20] = {0};
    DialogScaleShow(4, 12, 120, 32);

    Quit_Inspect();

    if(ButtonScan() == Up && (*item->data) < 90)(*item->data) += 1;
    if(ButtonScan() == Down && (*item->data) > 0)(*item->data) -= 1;
    SetForearmAngle((*item->data));
    sprintf(value, "ForearmAngle = %d", (*item->data));
    OLED_DrawStr(8, 25, value);
    Draw_Scrollbar(8, 30, 100, 6, 3, 0, 90, (*item->data));
    OLED_SendBuffer();
}

void Arm_ClampAngle(xpItem item)
{
    char value[20] = {0};
    DialogScaleShow(4, 12, 120, 32);

    Quit_Inspect();

    if(ButtonScan() == Up && (*item->data) < 180)(*item->data) += 1;
    if(ButtonScan() == Down && (*item->data) > 0)(*item->data) -= 1;
    SetClampAngle((*item->data));
    sprintf(value, "ClampAngle = %d", (*item->data));
    OLED_DrawStr(8, 25, value);
    Draw_Scrollbar(8, 30, 100, 6, 3, 0, 180, (*item->data));
    OLED_SendBuffer();
}

void ArmMotionSave(xpItem item)
{
    static uint8_t MotionID;
    char *MotionName = (char *)malloc(sizeof(char[20]));
    xpItem MotionItemTemp = (xpItem)malloc(sizeof(xItem));

    DialogScaleShow(4, 12, 120, 32);

    MotionSave(MotionItemTemp, ArmRobotAngle.Base_Angle, ArmRobotAngle.BigArm_Angle, ArmRobotAngle.Forearm_Angle, ArmRobotAngle.Clamp_Angle);
    MotionID ++;
    sprintf(MotionName," -Motion %d", MotionID);
    OLED_DrawStr(8, 25, "Successfully saved");
    OLED_DrawStr(8, 37, MotionName);
    OLED_SendBuffer();

    AddItem(MotionName, SWITCH, NULL, MotionItemTemp, &MotionTree_Page, NULL, Move);
    
    AppBreak();
}

void Move(xpItem item)
{
    pArmMotion current_motion = head; // 从第一个节点开始
    while (current_motion->angle_item != item) {
        current_motion = current_motion->next_motion; // 移到下一个节点
    }
    AngleMove(current_motion->base_angle, current_motion->bigArm_angle, current_motion->forearm_angle, current_motion->clamp_angle);
    AppBreak();
}

void MotionRun(xpItem item)
{
    DialogScaleShow(4, 12, 120, 32);
    OLED_DrawStr(8,25,"Motion runing");
    OLED_SendBuffer();
    Execute_Motions(item);
    AppBreak();
}

void ResetAngle(xpItem item)
{
    ArmRobotAngle.Base_Angle = 0;
    ArmRobotAngle.BigArm_Angle = 0;
    ArmRobotAngle.Forearm_Angle = 0;
    ArmRobotAngle.Clamp_Angle = 180;
    SetBaseAngle(ArmRobotAngle.Base_Angle);
    SetBigArmAngle(ArmRobotAngle.BigArm_Angle);
    SetForearmAngle(ArmRobotAngle.Forearm_Angle);
    SetClampAngle(ArmRobotAngle.Clamp_Angle);
    AppBreak();
}

void Execute_Motions(xpItem item)
{
    pArmMotion current_motion = head; // 从第一个节点开始
    for(;;)
    {
        while (current_motion != NULL) {
            if (current_motion->id > 0 && current_motion->angle_item->SwitchState == true)AngleMove(current_motion->base_angle, current_motion->bigArm_angle, current_motion->forearm_angle, current_motion->clamp_angle);
            if(Quit_Inspect() == true)return ;
            Serial.printf("Name: %s", current_motion->angle_item->ItemName);
            current_motion = current_motion->next_motion; // 移到下一个节点
        }
        current_motion = head;
    }
}

void Arm_SetX(xpItem item)
{
    char value[20] = {0};
    DialogScaleShow(4, 12, 120, 32);

    Quit_Inspect();

    if(ButtonScan() == Up)(*item->data)++;
    if(ButtonScan() == Down)(*item->data)--;
    sprintf(value, "AxisX = %d", (*item->data));
    OLED_DrawStr(8, 25, value);
    OLED_DrawRBox(8, 30, abs((*item->data)) + 6, 6, 3);
    OLED_DrawRFrame(8, 30, 90, 6, 3);
    OLED_SendBuffer();
}

void Arm_SetY(xpItem item)
{
    char value[20] = {0};
    DialogScaleShow(4, 12, 120, 32);

    Quit_Inspect();

    if(ButtonScan() == Up)(*item->data)++;
    if(ButtonScan() == Down)(*item->data)--;
    sprintf(value, "AxisY = %d", (*item->data));
    OLED_DrawStr(8, 25, value);
    OLED_DrawRBox(8, 30, abs((*item->data)) + 6, 6, 3);
    OLED_DrawRFrame(8, 30, 90, 6, 3);
    OLED_SendBuffer();
}

void Arm_SetZ(xpItem item)
{
    char value[20] = {0};
    DialogScaleShow(4, 12, 120, 32);

    Quit_Inspect();

    if(ButtonScan() == Up)(*item->data)++;
    if(ButtonScan() == Down)(*item->data)--;
    sprintf(value, "AxisZ = %d", (*item->data));
    OLED_DrawStr(8, 25, value);
    OLED_DrawRBox(8, 30, abs((*item->data)) + 6, 6, 3);
    OLED_DrawRFrame(8, 30, 90, 6, 3);
    OLED_SendBuffer();
}

void ArmAxis_Move(xpItem item)
{
    ARM_Control(ArmRobotAngle.AxisX, ArmRobotAngle.AxisY, ArmRobotAngle.AxisZ);
    Quit_Inspect();
}