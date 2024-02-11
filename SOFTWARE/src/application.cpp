#include "application.h"
#include "disp_dirver.h"
#include "ArmRobotConfig.h"

//1为白天模式，0为黑夜模式
extern uint8_t BgColor;  

bool IntoState = false;

void DialogScaleShow(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    if(IntoState==false)
    {
        DialogScale_Show(x,y,w,h);
        IntoState=true;
    }
    Draw_DialogBox(x,y,w,h);
}

//绘制滚动条函数
void Draw_Scrollbar(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t r, double min, double max, uint16_t NowValue)
{
    double value = NowValue * ((w - 6)/(max - min)) + 6;
    OLED_DrawRBox( x, y, (uint16_t)value, h, r);
    OLED_DrawRFrame( x, y, w, h, r);
}

bool Quit_Inspect(void)
{
    if(ButtonScan() == Lift)
    {
        Switch_Menu_State(MENU_RUN);
        if(IntoState == true)
        IntoState = false;
        return true;
    }
    return false;
}

void AppBreak(void)
{
    Switch_Menu_State(APP_BREAK);
    if(IntoState == true)
    IntoState = false;
}

void White_Dark_Day(xpItem item)
{
    BgColor = !item->SwitchState;
}

void Show_MPU6050(xpItem item)
{
    static float pitch, roll, yaw; 		//欧拉角
    char Pi[20], Ro[20], Ya[20];

    DialogScaleShow(1, 2, 100, 62);

    // mpu_dmp_get_data(&pitch, &roll, &yaw);

    sprintf(Pi, "Pitch = %.2f", pitch);
    sprintf(Ro, "Roll  = %.2f", roll);
    sprintf(Ya, "Yaw   = %.2f", yaw);

    OLED_DrawStr(12, 14, Pi);
    OLED_DrawStr(12, 36, Ro);
    OLED_DrawStr(12, 58, Ya);

    OLED_SendBuffer();

}

void Show_GitHub(xpItem item)
{
    const char* GitHub1="https://github.com/";
    const char* GitHub2="JFeng-Z/MultMenu";

    DialogScaleShow(2, 12, 125, 32);
    OLED_DrawStr(8, 25, GitHub1);
    OLED_DrawStr(8, 37, GitHub2);
    OLED_SendBuffer();
}

void Show_Bilibili(xpItem item)
{
    const char* Bilibili = "BV1Xh4y1N7By";

    DialogScaleShow(22, 24, 82, 20);

    OLED_DrawStr(28, 37, Bilibili);
    OLED_SendBuffer();
}

void Setting_Pid(xpItem item)
{
    char value[20] = {0};
    DialogScaleShow(4, 12, 120, 32);

    if((ButtonScan() == Up))(*item->data)+=10;
    if((ButtonScan() == Down))(*item->data)-=10;
    sprintf(value, "Value: %d", (*item->data));
    OLED_DrawStr(8, 25, value);
    Draw_Scrollbar(8, 30, 90, 6, 3, -1000, 1000, (*item->data));
    OLED_SendBuffer();
}

const unsigned char Log[] = { 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0x00,0x00,0x00,0x00,0x00,
0x00,0xF0,0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0x07,0x00,0x00,0x00,0x00,
0x80,0x3F,0xFF,0x1F,0x00,0x00,0x00,0x00,0xC0,0x03,0xFE,0x3F,0x00,0x00,0x00,0x00,
0x00,0x00,0xFE,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0x01,0x00,0x00,0x00,
0x00,0xF0,0xFF,0xFF,0x07,0x00,0x00,0x00,0x80,0xFF,0xFF,0xFF,0x0F,0x00,0x00,0x00,
0x00,0xFF,0xFF,0xFD,0x3F,0x00,0x00,0x00,0x00,0x00,0xFC,0xF8,0xFF,0x00,0x00,0x00,
0x00,0x00,0x7C,0xE0,0xFF,0x01,0x00,0x00,0x00,0x00,0x3C,0xC0,0xFF,0x07,0x00,0x00,
0x00,0x00,0x08,0x00,0xFF,0xEF,0x1C,0x00,0x00,0x00,0x00,0x00,0xFC,0x3F,0x60,0x00,
0x00,0x00,0x00,0x00,0xF8,0xFF,0xC3,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0x9F,0x00,
0x00,0x00,0x00,0x00,0xC0,0xFF,0x3F,0x01,0x00,0x00,0x00,0x00,0x00,0xFF,0x3F,0x01,
0x00,0x00,0x00,0x00,0x00,0xFE,0x7F,0x02,0x00,0x00,0x00,0x00,0x00,0xFE,0x7F,0x02,
0x00,0x00,0x00,0x00,0x80,0xFF,0x7F,0x02,0x00,0x00,0x00,0x00,0xE0,0xFF,0x7F,0x02,
0x00,0x00,0x00,0x00,0xF8,0xFF,0x7F,0x02,0x00,0x00,0x00,0x00,0xFE,0xFF,0x3F,0x01,
0x00,0x00,0x00,0x80,0xFF,0xFF,0x1F,0x01,0x00,0x00,0x00,0xE0,0xFF,0xFF,0x8F,0x00,
0x00,0x00,0x00,0xF0,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0xFC,0xFF,0x3F,0x20,0x00,
0x00,0x80,0x3F,0xFF,0xFF,0xEF,0x1C,0x00,0x00,0xE0,0xFF,0xFF,0xFF,0x03,0x00,0x00,
0x00,0xF0,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0x3F,0x00,0x00,0x00,
0x00,0xF8,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0xF8,0xFF,0xFF,0x03,0x00,0x00,0x00,
0x00,0xF8,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0xF8,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0xF0,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0xE0,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0x80,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0x80,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0xF8,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0xF8,0xFF,0xFF,0x03,0x00,0x00,0x00,
0x00,0xF8,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0xF8,0xFF,0xFF,0x03,0x00,0x00,0x00,
0x00,0xE0,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

void Show_Log(xpItem item)
{
    OLED_ClearBuffer();
    OLED_DrawXBMP(32,0,64,64,Log);
    OLED_SendBuffer();
}

void Setting_Contrast(xpItem item)
{
    char contrast[20];
    DialogScaleShow(4, 12, 120, 32);

    if((ButtonScan() == Up) && (*item->data) < 255)(*item->data)++;
    if((ButtonScan() == Down) && (*item->data) > 0)(*item->data)--;

    sprintf(contrast, "Contrast = %d", (*item->data));
    OLED_DrawStr(8, 25, contrast);

    Draw_Scrollbar(8, 30, 90, 6, 3, 0, 255, (*item->data));
    OLED_SetContrast((*item->data));
    OLED_SendBuffer();
}

void PowerSave(xpItem item)
{
    OLED_SetPowerSave(!item->SwitchState);
}

ArmRobot_Angle ArmRobotAngle = {0, 0, 0, 180, 0, 0, 0};
extern pArmMotion pm,head,tail; 
extern xPage MotionTree_Page;

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
    MotionItemTemp->SwitchState = false;
    MotionID ++;
    sprintf(MotionName," -Motion %d", MotionID);
    OLED_DrawStr(8, 25, "Successfully saved");
    OLED_DrawStr(8, 37, MotionName);
    OLED_SendBuffer();

    AddItem(MotionName, SWITCH, NULL, MotionItemTemp, &MotionTree_Page, NULL, Move);
}

void Move(xpItem item)
{
    pArmMotion current_motion = head; // 从第一个节点开始
    while (current_motion->angle_item != item) {
        current_motion = current_motion->next_motion; // 移到下一个节点
    }
    AngleMove(current_motion->base_angle, current_motion->bigArm_angle, current_motion->forearm_angle, current_motion->clamp_angle);
}

void MotionRun(xpItem item)
{
    DialogScaleShow(4, 12, 120, 32);
    OLED_DrawStr(8,25,"Motion runing");
    OLED_SendBuffer();
    Execute_Motions(item);
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
}

void Execute_Motions(xpItem item)
{
    pArmMotion current_motion = head; // 从第一个节点开始
    for(;;)
    {
        while (current_motion != NULL) {
            if (current_motion->id > 0 && current_motion->angle_item->SwitchState == true)AngleMove(current_motion->base_angle, current_motion->bigArm_angle, current_motion->forearm_angle, current_motion->clamp_angle);
            if(Quit_Inspect() == true)return ;
            current_motion = current_motion->next_motion; // 移到下一个节点
        }
        current_motion = head;
    }
}
