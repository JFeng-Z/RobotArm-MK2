#include "Menu.h"
#include "ArmRobotConfig.h"
#include "disp_dirver.h"

Menu_State Page_State;
//1为白天模式，0为黑夜模式
uint8_t BgColor = 0x00;  
//选项缓动动画持续时间（次数）
int Options_Time = 8;
//对话框缓动动画持续时间（次数）
uint8_t Dialog_Time = 10;

Pid_Error Cursor;

/* Page*/
xPage
    Home_Page, 
    System_Page, ArmAxis_Page, ArmAngle_Page, MotionTree_Page, Games_Page;
/* item */
xItem Main_Item, System_Item, ArmAxis_Item, ArmAngle_Item, Games_Item, ShowLogo_Item, Github_Item, Bilibili_Item, ReadME_Item;
xItem MPU6050_Item, Speed_Item, Mode_Item, Contrast_Item, Power_Item, SystemReturn_Item;
xItem Arm_ItemX, Arm_ItemY, Arm_ItemZ, AxisMove_Item, ArmAxisReturn_Item;
xItem BaseAngle_Item, BigArmAngle_Item, ForearmAngle_Item, ClampAngle_Item, ArmAngleReturn_Item, AngleMove_Item, MotionTree_Item, MotionTreeReturn_Item, ExecuteMotions_Item, Reset_Item;
xItem Dino_Item, AirPlane_Item, GamesReturn_Item;

void AddPage(const char *name, xpPage page)
{
    page->PageName = name;
    page->itemHead = NULL;
    page->itemTail = NULL;
}

void AddItem(const char *Name, Item_Type Type, int *Data, xpItem item, xpPage LocalPage, xpPage nextpage, Itemfunction function)
{
    item->ItemName = Name;
    item->ItemType = Type;
    if(Type == DATA)item->data = Data;
    item->location = LocalPage;
    item->item_function = function;
    /* 新建item的下一个肯定是null */
    item->nextiTem = NULL;
    /* 如果可以跳转，那么此item是跳转页面的父级 */
    if (nextpage != NULL)
    nextpage->ParentiTem = item;
    else nextpage = LocalPage;
    item->JumpPage = nextpage;
    /* 链式结构创建item */
    if (LocalPage->itemHead == NULL) // 如果是第一个iTem
    {
        item->lastiTem = item;
        LocalPage->itemHead = item;
        LocalPage->itemTail = item;
        LocalPage->len = 1;
    }
    else // 不是第一个item
    {
        item->lastiTem = LocalPage->itemTail;                // 新item的last指向Local的tailitem
        LocalPage->itemTail->nextiTem = item;                // 让尾巴的next指向新的item，连接起来
        LocalPage->itemTail = LocalPage->itemTail->nextiTem; // 让尾巴指向新的item
        LocalPage->len++;
    }
    item->id = LocalPage->len;
}

/**
 * @brief 线性增长函数用于坐标移动
 * 
 * @param AllTime 总时长
 * @param Time_Now 当前时间
 * @param Targrt 目标值
 * @param Now 当前值
 * @return uint8_t 
 */
int8_t Line(uint8_t AllTime, uint8_t Time_Now, int8_t Targrt, int8_t Now)
{
    return (Targrt - Now)*Time_Now/AllTime + Now;		
}

uint8_t PID(int8_t Targrt, int8_t Now, Pid_Error *Obj)
{
    int x = Now;
    float Kp = 0.5,ki = 0.1,kd = 0.25;
    Obj->error = Targrt - x;
    Obj->sum_srror += Obj->error;
    float delta_error = Obj->error - Obj->last_error;
    float velocity = Kp * Obj->error + ki * Obj->sum_srror + kd * delta_error;
    x += velocity;
    Obj->last_error = Obj->error;
    return x;
}
//首页
void Draw_Home(void)
{
    OLED_SetFont(u8g2_font_10x20_me);
    for (size_t i = 0; i < 40; i++)
    {
        OLED_ClearBuffer();
        OLED_DrawStr(25, i, "MultMenu");
        OLED_SendBuffer();
    }
}

void Draw_DialogBox(u8g2_uint_t x,u8g2_uint_t y,u8g2_uint_t w,u8g2_uint_t h)
{
    OLED_SetDrawColor(BgColor^0x01);
    OLED_DrawFrame(x, y, w, h);
    OLED_SetDrawColor(BgColor);
    OLED_DrawBox(x+1, y+1, w-2, h-2);
    OLED_SetDrawColor(BgColor^0x01);
}

void Draw_DialogRBox(u8g2_uint_t x,u8g2_uint_t y,u8g2_uint_t w,u8g2_uint_t h,u8g2_uint_t r)
{
    OLED_SetDrawColor(BgColor^0x01);
    OLED_DrawRFrame(x, y, w, h, r);
    OLED_SetDrawColor(BgColor);
    OLED_DrawRBox(x+1, y+1, w-2, h-2, r);
    OLED_SetDrawColor(BgColor^0x01);
}
/**
 * @brief 对话框出现函数
 * 
 * @param u8g2 U8G2
 * @param x 初始位置x
 * @param y 初始位置y
 * @param Targrt_w 目标宽度
 * @param Targrt_h 目标高度
 */
void DialogScale_Show(uint8_t x,uint8_t y,uint8_t Targrt_w,uint8_t Targrt_h)
{
    uint8_t t = 0;
    uint8_t Init_w = 0,Init_h = 0;

    do
    {
        t++;
        Init_w=Line(Dialog_Time, t, Targrt_w, Init_w);
        Init_h=Line(Dialog_Time, t, Targrt_h, Init_h);
        Draw_DialogBox(x, y, Init_w, Init_h);
        OLED_SendBuffer();
    } while (t < Dialog_Time);

}
/**
 * @brief 渐变消失函数
 * 
 * 
 */
uint8_t ui_disapper(uint8_t disapper)
{ 
    short disapper_temp = 0;
    int len = 8 * OLED_GetBufferTileHeight() * OLED_GetBufferTileWidth();
    u8 *p = OLED_GetBufferPtr(); 
    if(BgColor==0)
    {  for( int i = 0;i < len ;i++) 
    { p[i] = p[i] & (rand()%0xff) >> disapper; } }
    else
    {  for( int i = 0;i < len ;i++) 
    { p[i] = p[i] | (rand()%0xff) >> disapper; } }
    disapper += 2; 
    if(disapper >= 8) 
    {disapper = 0; } 
    OLED_SendBuffer();
    disapper_temp = disapper;
    return disapper_temp;
}

/**
 * @brief 导航栏
 * 
 * @param now_time 
 * @param now_item 
 * @param next_item 
 */
void Draw_OptionPlace(uint8_t now_time, xpItem now_item, xpItem next_item)
{
    static uint8_t Now_Lenght;
    static uint8_t Next_Lenght;
    Next_Lenght = (VER_RES / (float)(next_item->location->len)) * next_item->id;
    Now_Lenght = Line(Options_Time, now_time, Next_Lenght, Now_Lenght);
    OLED_DrawLine(HOR_RES - 7, 0, HOR_RES - 7, 64);
    OLED_DrawBox(HOR_RES - 10, 0, 6, Now_Lenght);
}

void Draw_Page(uint8_t pos, xpPage Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item)
{
    char Data[10] = {0};
    static int16_t first_line = FirstLine;
    xpItem temp = Page->itemHead;
    //切换页面时变量初始化
    if(next_item == now_item->JumpPage->itemHead && next_item != now_item)first_line = FirstLine;

    if (Page_State == CURSOR_STATIC)
    {
        if ((next_item->id - now_item->id) > 0)first_line -= ((next_item->id - now_item->id) > (Page->len - MaxVisible_Number)) ? ((Page->len - MaxVisible_Number) * Font_Size) : Font_Size;
        else first_line += ((now_item->id - next_item->id) > (Page->len - MaxVisible_Number)) ? ((Page->len - MaxVisible_Number) * Font_Size) : Font_Size;
        Page_State = MENU_MOVE;
    }

    for (uint16 i = 1; i <= Page->len; i++)
    {
        if((first_line + i * LineSpacing) > FirstLine)      //菜单移动时不让标题移动
        {
            OLED_DrawStr(pos, first_line + i * LineSpacing, temp->ItemName);
            if(temp->ItemType == SWITCH)
            {
                if(temp->SwitchState == false)OLED_DrawFrame(pos + 95, first_line + i * LineSpacing - Font_Size + 3, 10, 10);
                else OLED_DrawBox(pos + 95, first_line + i * LineSpacing - Font_Size + 3, 10, 10);
            }
            if(temp->ItemType == DATA)
            {
                sprintf(Data, "%d", *temp->data);
                OLED_DrawStr(pos + 95, first_line + i * LineSpacing, Data);
            }
        }
        temp = temp->nextiTem;
    }
    OLED_DrawStr(pos, FirstLine, Page->PageName); 
}

void Draw_Menu(uint8_t pos, xpPage Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item)
{
    uint8_t t = 0;
    uint8_t item_wide = strlen(now_item->ItemName)*6 + 4;
    static uint8_t item_line = LINE_MIN;
    static int16_t Targrt_line = 0;
    static uint8_t first = 0;     //初始状态

    OLED_SetMaxClipWindow();
    OLED_SetFont(MENU_FONT);

    Page->itemTail->nextiTem = Page->itemHead;  
    Page->itemHead->lastiTem = Page->itemTail;
    
    if(next_item == now_item->JumpPage->itemHead)        //切换页面时变量初始化
    {
        item_line = LINE_MIN;
        Targrt_line = 0;
        first = 0;
        Page_State = MENU_RUN;
    }
    if (((next_item->id - now_item->id) == 0 && first == 0) || next_item == now_item->JumpPage->itemHead)
    {
        Targrt_line = LINE_MIN;
        first = 1;
    }
    else if ((next_item->id - now_item->id) > 0)
    {
        Targrt_line += ((next_item->id-now_item->id)*Font_Size);
        if (Targrt_line > LINE_MAX)  //防止光标溢出可视范围
        {
            Page_State = CURSOR_STATIC;
            Targrt_line = LINE_MAX;
        }
    }
    else if((next_item->id - now_item->id) < 0)
    {
        Targrt_line -= ((now_item->id-next_item->id)*Font_Size);
        if (Targrt_line < LINE_MIN)  //防止光标溢出可视范围
        {
            Page_State = CURSOR_STATIC;
            Targrt_line = LINE_MIN;
        }
    }

    do
    {
        OLED_ClearBuffer();
        t++;
        OLED_SetDrawColor(BgColor);
        OLED_DrawBox(0, 0, 128, 64);
        OLED_SetDrawColor(BgColor^0x01);
        Draw_OptionPlace(t, now_item, next_item);
        Draw_Page(pos, Page, LineSpacing, now_item, next_item);
        OLED_SetDrawColor(2);
        item_line = PID(Targrt_line, item_line, &Cursor);
        if(t >= Options_Time)item_line = Targrt_line;
        item_wide = PID(strlen(next_item->ItemName)*6 + 4, item_wide, &Cursor);
        if(t >= Options_Time)item_wide = strlen(next_item->ItemName)*6 + 4;
        OLED_DrawRBox(pos+1, item_line-1, item_wide, Font_Size, 4);
        OLED_SendBuffer();
    } while (t < Options_Time);
}

int Contrast = 255;
extern ArmRobot_Angle ArmRobotAngle;

void Menu_Team(void)
{
    AddPage("[HomePage]", &Home_Page);
        AddItem(" +System", PARENTS, NULL, &System_Item, &Home_Page, &System_Page, NULL);
            AddPage("[System]", &System_Page);
                AddItem(" -MPU6050", CUSTOM, NULL, &MPU6050_Item, &System_Page, NULL, Show_MPU6050);
                AddItem(" -Speed", DATA, &Options_Time, &Speed_Item, &System_Page, NULL, Setting_Speed);
                AddItem(" -Mode", SWITCH, NULL, &Mode_Item, &System_Page, NULL, White_Dark_Day);
                AddItem(" -Contrast", DATA, &Contrast, &Contrast_Item, &System_Page, NULL, Setting_Contrast);
                AddItem(" -Power", SWITCH, NULL, &Power_Item, &System_Page, NULL, PowerSave);
                AddItem(" -Return", PARENTS, NULL, &SystemReturn_Item, &System_Page, &Home_Page, NULL);
        AddItem(" +ArmAxis", PARENTS, NULL, &ArmAxis_Item,&Home_Page,&ArmAxis_Page,NULL);
            AddPage("[ArmAxis]", &ArmAxis_Page);
                AddItem(" -X", DATA, &ArmRobotAngle.AxisX, &Arm_ItemX, &ArmAxis_Page, NULL, Arm_SetX);
                AddItem(" -Y", DATA, &ArmRobotAngle.AxisY, &Arm_ItemY, &ArmAxis_Page, NULL, Arm_SetY);
                AddItem(" -Z", DATA, &ArmRobotAngle.AxisZ, &Arm_ItemZ, &ArmAxis_Page, NULL, Arm_SetZ);
                AddItem(" -Move", CUSTOM, NULL, &AxisMove_Item, &ArmAxis_Page, NULL, ArmAxis_Move);
                AddItem(" -Return", PARENTS, NULL, &ArmAxisReturn_Item, &ArmAxis_Page, &Home_Page, NULL);
        AddItem(" +ArmAngle",PARENTS, NULL, &ArmAngle_Item, &Home_Page,&ArmAngle_Page,NULL);
            AddPage("[ArmAngle]", &ArmAngle_Page);
                AddItem(" -Base", DATA, &ArmRobotAngle.Base_Angle, &BaseAngle_Item, &ArmAngle_Page, NULL, Arm_SetBaseAngle);
                AddItem(" -BigArm", DATA, &ArmRobotAngle.BigArm_Angle, &BigArmAngle_Item, &ArmAngle_Page, NULL, Arm_SetBigArmAngle);
                AddItem(" -Forearm", DATA, &ArmRobotAngle.Forearm_Angle, &ForearmAngle_Item, &ArmAngle_Page, NULL, Arm_SetForearmAngle);
                AddItem(" -Clamp", DATA, &ArmRobotAngle.Clamp_Angle, &ClampAngle_Item, &ArmAngle_Page, NULL, Arm_ClampAngle);
                AddItem(" -Motion Save", CUSTOM, NULL, &AngleMove_Item, &ArmAngle_Page, NULL, ArmMotionSave);
                AddItem(" -Motion Tree", PARENTS, NULL, &MotionTree_Item, &ArmAngle_Page, &MotionTree_Page, NULL);
                    AddPage("[Motion Tree]", &MotionTree_Page);
                    /*动作树*/
                    AddItem(" ---", PARENTS, NULL, &MotionTreeReturn_Item, &MotionTree_Page, &ArmAngle_Page, NULL);
                    AddItem(" -Loop", CUSTOM, NULL, &ExecuteMotions_Item, &MotionTree_Page, NULL, MotionRun);
                AddItem(" -Reset", CUSTOM, NULL, &Reset_Item, &ArmAngle_Page, NULL, ResetAngle);
                AddItem(" -Return", PARENTS, NULL, &ArmAngleReturn_Item, &ArmAngle_Page, &Home_Page, NULL);
        AddItem(" -ShowLogo", CUSTOM, NULL, &ShowLogo_Item, &Home_Page, NULL, Show_Log);
        AddItem(" -Github", CUSTOM, NULL, &Github_Item, &Home_Page, NULL, Show_GitHub);
        AddItem(" -Bilibili", CUSTOM, NULL, &Bilibili_Item, &Home_Page, NULL, Show_Bilibili);
}

Menu_State MENU_STATE = MENU_RUN;
//初始化为开始菜单项
xpItem temp_item = &System_Item;

void Switch_Menu_State(Menu_State state)
{
    MENU_STATE = state;
}

Menu_State BtnScan(void)
{
    if(ButtonScan()==Up)return MENU_UP;
    else if(ButtonScan()==Down)return MENU_DOWN;
    else if(ButtonScan()==Center)return MENU_ENTER;
    return MENU_NONE;
}

void Process_Menu_Run(Menu_State Dir)
{
    uint8_t disapper = 1;
    switch (Dir)
    {
        case MENU_UP:
            Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item->lastiTem);
            temp_item = temp_item->lastiTem;
            break;
        case MENU_DOWN:
            Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item->nextiTem);
            temp_item = temp_item->nextiTem;
            break;
        case MENU_ENTER:
            if (MENU_STATE == MENU_RUN)
            {
                if(temp_item->ItemType == PARENTS)
                {
                    Switch_Menu_State(MENU_RUN);
                    for (size_t i = 0; i < 8; i++)
                    {
                        disapper = ui_disapper(disapper);
                    }
                    Draw_Menu(FirstPos, temp_item->JumpPage, Font_Size, temp_item, temp_item->JumpPage->itemHead);
                    temp_item = temp_item->JumpPage->itemHead;
                }
                else
                {
                    ui_disapper(disapper);
                    Switch_Menu_State(APP_RUN);
                }
            }
            if(MENU_STATE == APP_BREAK)
            {
                Switch_Menu_State(MENU_RUN);
                for (size_t i = 0; i < 8; i++)
                {
                    disapper = ui_disapper(disapper);
                }
                Draw_Menu(FirstPos, temp_item->location, Font_Size, temp_item, temp_item);
            }
            break;
        default:
            break;
    }
}

void Process_App_Run(xpItem item)
{
    if (item->ItemType == SWITCH)
    {
        item->SwitchState = ! item->SwitchState;
        AppBreak();
    }
    (item->item_function)(item);
}

void Menu_Task(void)
{
    Menu_State Dir = BtnScan();
    static bool Start;

    if (Start == true)
    {
        switch (MENU_STATE)
        {
            case MENU_RUN:
            case APP_BREAK:
                Process_Menu_Run(Dir);
                break;

            case APP_RUN:
                Process_App_Run(temp_item);
                break;
                
            default:
                break;
        }
    }

    if (Dir != MENU_NONE && Start == false)
    {
        Draw_Menu(FirstPos, &Home_Page, Font_Size, &System_Item, &System_Item);
        Start = true;
    }
    
}

void Menu_Init(void)
{
    disp_init();
    Menu_Team();
    Draw_Home();
}