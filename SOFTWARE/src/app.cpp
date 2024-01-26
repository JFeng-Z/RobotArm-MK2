#include "app.h"

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
//1为白天模式，0为黑夜模式
extern uint8_t BgColor;  

bool IntoState=false;

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
    u8g2.drawRBox( x, y, (u8g2_uint_t)value, h, r);
    u8g2.drawRFrame( x, y, w, h, r);
}

bool Quit_Inspect(void)
{
    if(BtnScan()==MENU_ENTER)
    {
        Switch_Menu_State(APP_BREAK);
        if(IntoState==true)
        IntoState=false;
        return true;
    }
    return false;
}

void AppBreak(void)
{
    Switch_Menu_State(APP_BREAK);
    if(IntoState==true)
    IntoState=false;
}

void White_Dark_Day(void)
{
    BgColor=BgColor^0x01;
    AppBreak();
}

void Show_MPU6050(void)
{
    static float pitch,roll,yaw; 		//欧拉角

    DialogScaleShow(1,2,100,62);

    Quit_Inspect();

    // mpu_dmp_get_data(&pitch,&roll,&yaw);
    u8g2.setCursor(12,14);
    u8g2.printf("Pitch = %.2f",pitch);

    u8g2.setCursor(12,36);
    u8g2.printf("Roll = %.2f",roll);

    u8g2.setCursor(12,58);
    u8g2.printf("Yaw = %.2f",yaw);

    u8g2.sendBuffer();

}

void Show_GitHub(void)
{
    const char* GitHub1="https://github.com/";
    const char* GitHub2="JFeng-Z/MultMenu";

    DialogScaleShow(2,12,125,32);
    u8g2.drawStr(8,25,GitHub1);
    u8g2.drawStr(8,37,GitHub2);
    u8g2.sendBuffer();
    AppBreak();
}

void Show_Bilibili(void)
{
    const char* Bilibili="BV1Xh4y1N7By";

    DialogScaleShow(22,24,82,20);

    u8g2.drawStr(28,37,Bilibili);
    u8g2.sendBuffer();
    AppBreak();
}

extern uint8_t Options_Time;

void Setting_Speed(void)
{
    DialogScaleShow(4,12,120,32);

    Quit_Inspect();

    if((ButtonScan()==Up)&&Options_Time<30)Options_Time++;
    if((ButtonScan()==Down)&&Options_Time>2)Options_Time--;
    u8g2.setCursor(8,25);
    u8g2.printf("Speed = %d",Options_Time);
    u8g2.drawRBox(8,30,Options_Time*3,6,3);
    u8g2.drawRFrame(8,30,90,6,3);
    u8g2.sendBuffer();
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

void Show_Log(void)
{
    u8g2.clearBuffer();
    u8g2.drawXBMP(32,0,64,64,Log);
    u8g2.sendBuffer();
    AppBreak();
}

int x=0,y=0,z=0;

void Arm_SetX(void)
{
    DialogScaleShow(4,12,120,32);

    Quit_Inspect();

    if(ButtonScan()==Up)x++;
    if(ButtonScan()==Down)x--;
    u8g2.setCursor(8,25);
    u8g2.printf("X = %d",x);
    u8g2.drawRBox(8,30,abs(x)+6,6,3);
    u8g2.drawRFrame(8,30,90,6,3);
    u8g2.sendBuffer();
}

void Arm_SetY(void)
{
    DialogScaleShow(4,12,120,32);

    Quit_Inspect();

    if(ButtonScan()==Up)y++;
    if(ButtonScan()==Down)y--;
    u8g2.setCursor(8,25);
    u8g2.printf("Y = %d",y);
    u8g2.drawRBox(8,30,abs(y)+6,6,3);
    u8g2.drawRFrame(8,30,90,6,3);
    u8g2.sendBuffer();
}

void Arm_SetZ(void)
{
    DialogScaleShow(4,12,120,32);

    Quit_Inspect();

    if(ButtonScan()==Up)z++;
    if(ButtonScan()==Down)z--;
    u8g2.setCursor(8,25);
    u8g2.printf("Z = %d",z);
    u8g2.drawRBox(8,30,abs(z)+6,6,3);
    u8g2.drawRFrame(8,30,90,6,3);
    u8g2.sendBuffer();
}

void ArmAxis_Move(void)
{
    ARM_Control(x,y,z);
    Quit_Inspect();
}

uint8_t Base_Angle,BigArm_Angle,Forearm_Angle;

void Arm_SetBaseAngle(void)
{
    DialogScaleShow(0,12,127,32);

    Quit_Inspect();

    if(ButtonScan()==Up && Base_Angle < 180)Base_Angle+=1;
    if(ButtonScan()==Down && Base_Angle > 0)Base_Angle-=1;
    SetBaseAngle(Base_Angle);
    u8g2.setCursor(8,25);
    u8g2.printf("BaseAngle = %d",Base_Angle);
    Draw_Scrollbar(8,30,100,6,3,0,180,Base_Angle);
    u8g2.sendBuffer();
}

void Arm_SetBigArmAngle(void)
{
    DialogScaleShow(4,12,120,32);

    Quit_Inspect();

    if(ButtonScan()==Up && BigArm_Angle < 180)BigArm_Angle+=1;
    if(ButtonScan()==Down && BigArm_Angle > 0)BigArm_Angle-=1;
    SetBigArmAngle(BigArm_Angle);
    u8g2.setCursor(8,25);
    u8g2.printf("BigArm = %d",BigArm_Angle);
    Draw_Scrollbar(8,30,100,6,3,0,180,BigArm_Angle);
    u8g2.sendBuffer();
}

void Arm_SetForearmAngle(void)
{
    DialogScaleShow(4,12,120,32);

    Quit_Inspect();

    if(ButtonScan()==Up && Forearm_Angle < 90)Forearm_Angle+=1;
    if(ButtonScan()==Down && Forearm_Angle > 0)Forearm_Angle-=1;
    SetForearmAngle(Forearm_Angle);
    u8g2.setCursor(8,25);
    u8g2.printf("Forearm = %d",Forearm_Angle);
    Draw_Scrollbar(8,30,100,6,3,0,90,Forearm_Angle);
    u8g2.sendBuffer();
}

extern xMenu MotionTree_Page;
extern xpItem temp_item;
extern pArmMotion head;

void Move(void)
{
    pArmMotion current_motion = head; // 从第一个节点开始
    while (current_motion->id != (temp_item->id - 2)) {
        current_motion = current_motion->next_motion; // 移到下一个节点
    }
    AngleMove(current_motion->base_angle, current_motion->bigArm_angle, current_motion->forearm_angle);
    AppBreak();
}

void ArmMotionSave(void)
{
    static uint8_t MotionID;
    char *MotionName = (char *)malloc(sizeof(char[20]));
    xpItem MotionItemTemp = (xpItem)malloc(sizeof(xItem));

    DialogScaleShow(4,12,120,32);

    MotionSave(Base_Angle,BigArm_Angle,Forearm_Angle);
    MotionID ++;
    u8g2.setCursor(8, 25);
    u8g2.printf("Successfully saved");
    u8g2.setCursor(8, 37);
    sprintf(MotionName," -Motion %d",MotionID);
    u8g2.printf("Motion:%d",MotionID);
    u8g2.sendBuffer();

    AddItem(MotionName, MotionItemTemp, &MotionTree_Page, NULL, Move);
    
    AppBreak();
}

void MotionRun(void)
{
    DialogScaleShow(4,12,120,32);
    u8g2.drawStr(8,25,"Motion runing");
    u8g2.sendBuffer();
    Execute_Motions();
    AppBreak();
}

void ResetAngle(void)
{
    Base_Angle = 0;
    BigArm_Angle = 0;
    Forearm_Angle = 0;
    SetBaseAngle(Base_Angle);
    SetBigArmAngle(BigArm_Angle);
    SetForearmAngle(Forearm_Angle);
    AppBreak();
}
