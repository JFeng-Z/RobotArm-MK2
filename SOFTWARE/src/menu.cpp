#include "Menu.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE, SCL, SDA);

uint8_t Page_State = 0;
//1为白天模式，0为黑夜模式
uint8_t BgColor = 0x00;  
//选项缓动动画持续时间（次数）
uint8_t Options_Time = 8;
//对话框缓动动画持续时间（次数）
uint8_t Dialog_Time = 10;

Error Cursor;

/* Page*/
xMenu
    Home_Page, 
    System_Page, ArmAxis_Page, ArmAngle_Page, Games_Page;
/* item */
xItem Main_Item, System_Item, ArmAxis_Item, ArmAngle_Item, Games_Item, ShowLogo_Item, Github_Item, Bilibili_Item, ReadME_Item;
xItem MPU6050_Item, Speed_Item, Mode_Item, Clock_Item, SystemReturn_Item;
xItem Arm_ItemX, Arm_ItemY, Arm_ItemZ, AxisMove_Item, ArmAxisReturn_Item;
xItem BaseAngle_Item, BigArmAngle_Item, ForearmAngle_Item, ArmAngleReturn_Item, AngleMove_Item, ExecuteMotions_Item, Reset_Item;
xItem Dino_Item, AirPlane_Item, GamesReturn_Item;

void AddPage(const char *name, xpMenu page)
{
    page->PageName = name;
    page->itemHead = NULL;
    page->itemTail = NULL;
}

void AddItem(const char *Name, xpItem item, xpMenu LocalPage, xpMenu nextpage, Itemfunction function)
{
    item->itemName = Name;
    item->location = LocalPage;
    item->JumpPage = nextpage;
    item->Itemfunction = function;
    /* 新建item的下一个肯定是null */
    item->nextiTem = NULL;
    /* 如果可以跳转，那么此item是跳转页面的父级 */
    if (nextpage != NULL)
        nextpage->ParentiTem = item;
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

uint8_t PID(int8_t Targrt, int8_t Now, Error *Obj)
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

void Draw_Process(void)
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont15_mf);
    u8g2.drawStr(32, 16, "Mr.JFeng");
    u8g2.setFont(MENU_FONT);
    
    for(size_t i = 10; i <= 80; i += 2)
    {
        char buff[6];
        int percentage = (int)(i/80.0*100);
        sprintf(buff, "%02d%%", percentage);
        
        u8g2.drawStr(100, 41, buff);
        u8g2.drawRBox(16, 32, i, 10, 4);
        u8g2.drawRFrame(16, 32, 80, 10, 4);
        u8g2.sendBuffer();
    }
}

void Draw_DialogBox(u8g2_uint_t x,u8g2_uint_t y,u8g2_uint_t w,u8g2_uint_t h)
{
    u8g2.setDrawColor(BgColor^0x01);
    u8g2.drawFrame(x, y, w, h);
    u8g2.setDrawColor(BgColor);
    u8g2.drawBox(x+1, y+1, w-2, h-2);
    u8g2.setDrawColor(BgColor^0x01);
}

void Draw_DialogRBox(u8g2_uint_t x,u8g2_uint_t y,u8g2_uint_t w,u8g2_uint_t h,u8g2_uint_t r)
{
    u8g2.setDrawColor(BgColor^0x01);
    u8g2.drawRFrame(x, y, w, h, r);
    u8g2.setDrawColor(BgColor);
    u8g2.drawRBox(x+1, y+1, w-2, h-2, r);
    u8g2.setDrawColor(BgColor^0x01);
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
void DialogScale_Show(uint16_t x,uint16_t y,uint16_t Targrt_w,uint16_t Targrt_h)
{
    uint8_t t = 0;
    uint16_t Init_w = 0,Init_h = 0;

    do
    {
        t++;
        Init_w=Line(Dialog_Time, t, Targrt_w, Init_w);
        Init_h=Line(Dialog_Time, t, Targrt_h, Init_h);
        Draw_DialogBox(x, y, Init_w, Init_h);
        u8g2.sendBuffer();
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
    int len = 8 * u8g2.getBufferTileHeight() * u8g2.getBufferTileWidth();
    u8 *p = u8g2.getBufferPtr(); 
    if(BgColor==0)
    {  for( int i = 0;i < len ;i++) 
    { p[i] = p[i] & (rand()%0xff) >> disapper; } }
    else
    {  for( int i = 0;i < len ;i++) 
    { p[i] = p[i] | (rand()%0xff) >> disapper; } }
    disapper += 2; 
    if(disapper >= 8) 
    {disapper = 0; } 
    u8g2.sendBuffer();
    disapper_temp = disapper;
    return disapper_temp;
}
/**
 * @brief 选项栏
 * 
 * @param now_time 
 * @param now_item 
 * @param next_item 
 */
void Draw_OptionPlace(uint8_t now_time, xpItem now_item, xpItem next_item)
{
    static uint8_t now_Y = 0;
    static uint8_t next_Y = 0;
    next_Y=(next_item->id - 1)*(64/next_item->location->len);
    u8g2.drawVLine(122, 2, 64);
    for (size_t i = 0; i < next_item->location->len; i++)
    {
        u8g2.drawHLine(119, i*(64/next_item->location->len)+2, 6);
    }
    now_Y=Line(Options_Time, now_time, next_Y, now_Y);
    u8g2.drawBox(118, now_Y, 8, 4);
}

void Draw_Page(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item)
{
    static int8_t first_line = FirstLine;
    xpItem temp = Page->itemHead;
    
    if(next_item == now_item->JumpPage->itemHead && next_item != now_item)    //切换页面时变量初始化
    first_line = FirstLine;
    if (((next_item->id - now_item->id)>0) && Page_State == CURSOR_STATIC)
    {
        Page_State = MENU_MOVE;
        if((next_item->id - now_item->id) > (Page->len - MaxVisible_Number))
        first_line -= ((Page->len-MaxVisible_Number)*Font_Size);  //除去不移动时的项目数
        else first_line -= Font_Size;
    }
    else if (((next_item->id - now_item->id) < 0) && Page_State == CURSOR_STATIC)
    {
        Page_State = MENU_MOVE;
        if((now_item->id - next_item->id) > (Page->len - MaxVisible_Number))
        first_line += ((Page->len-MaxVisible_Number)*Font_Size);  //除去不移动时的项目数
        else first_line += Font_Size;
    }
    for (size_t i = 1; i <= Page->len; i++)
    {
        if((first_line + i * LineSpacing) <= FirstLine);      //菜单移动时不让标题移动
        else u8g2.drawStr(pos, first_line + i * LineSpacing, temp->itemName);
        temp = temp->nextiTem;
    }
    u8g2.drawStr(pos, FirstLine, Page->PageName); 
  
}

void Draw_Menu(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item)
{
    uint8_t t = 0;
    uint8_t item_wide = strlen(now_item->itemName)*6+4;
    static uint8_t item_line = LINE_MIN;
    static int8_t Targrt_line = 0;
    static uint8_t first = 0;     //初始状态

    u8g2.setMaxClipWindow();
    u8g2.setFont(MENU_FONT);
    
    if(next_item == now_item->JumpPage->itemHead && next_item != now_item)        //切换页面时变量初始化
    {
        item_line = LINE_MIN;
        Targrt_line = 0;
        first = 0;
        Page_State = 0;
    }
    if (((next_item->id - now_item->id) == 0 && first == 0) || next_item == now_item->JumpPage->itemHead)
    {
        Targrt_line = LINE_MIN;
        first = 1;
    }
    else if ((next_item->id - now_item->id)>0)
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
    Page->itemTail->nextiTem = Page->itemHead;  
    Page->itemHead->lastiTem = Page->itemTail;
    do
    {
        u8g2.clearBuffer();
        t++;
        u8g2.setDrawColor(BgColor);
        u8g2.drawBox(0, 0, 128, 64);
        u8g2.setDrawColor(BgColor^0x01);
        Draw_OptionPlace(t, now_item, next_item);
        Draw_Page(pos, Page, LineSpacing, now_item, next_item);
        u8g2.setDrawColor(2);
        item_line = PID(Targrt_line, item_line, &Cursor);
        if(t >= Options_Time)item_line = Targrt_line;
        item_wide = PID(strlen(next_item->itemName)*6+4, item_wide, &Cursor);
        if(t >= Options_Time)item_wide = strlen(next_item->itemName)*6+4;
        u8g2.drawRBox(pos+1, item_line-1, item_wide, Font_Size, 4);
        u8g2.sendBuffer();
    } while (t < Options_Time);
}

void Menu_Team(void)
{
    AddPage("[HomePage]", &Home_Page);
        AddItem(" +System", &System_Item, &Home_Page, &System_Page, NULL);
            AddPage("[System]", &System_Page);
                AddItem(" -MPU6050", &MPU6050_Item, &System_Page, &System_Page, Show_MPU6050);
                AddItem(" -Speed", &Speed_Item, &System_Page, &System_Page, Setting_Speed);
                AddItem(" -Mode", &Mode_Item, &System_Page, &System_Page, White_Dark_Day);
                AddItem(" -Clock", &Clock_Item, &System_Page, &System_Page, NULL);
                AddItem(" -Return", &SystemReturn_Item, &System_Page, &Home_Page, NULL);
        AddItem(" +ArmAxis",&ArmAxis_Item,&Home_Page,&ArmAxis_Page,NULL);
            AddPage("[ArmAxis]", &ArmAxis_Page);
                AddItem(" -X", &Arm_ItemX, &ArmAxis_Page, &ArmAxis_Page, Arm_SetX);
                AddItem(" -Y", &Arm_ItemY, &ArmAxis_Page, &ArmAxis_Page, Arm_SetY);
                AddItem(" -Z", &Arm_ItemZ, &ArmAxis_Page, &ArmAxis_Page, Arm_SetZ);
                AddItem(" -Move", &AxisMove_Item, &ArmAxis_Page, &ArmAxis_Page, ArmAxis_Move);
                AddItem(" -Return", &ArmAxisReturn_Item, &ArmAxis_Page, &Home_Page, NULL);
        AddItem(" +ArmAngle",&ArmAngle_Item,&Home_Page,&ArmAngle_Page,NULL);
            AddPage("[ArmAngle]", &ArmAngle_Page);
                AddItem(" -Base", &BaseAngle_Item, &ArmAngle_Page, &ArmAngle_Page, Arm_SetBaseAngle);
                AddItem(" -BigArm", &BigArmAngle_Item, &ArmAngle_Page, &ArmAngle_Page, Arm_SetBigArmAngle);
                AddItem(" -Forearm", &ForearmAngle_Item, &ArmAngle_Page, &ArmAngle_Page, Arm_SetForearmAngle);
                AddItem(" -Move", &AngleMove_Item, &ArmAngle_Page, &ArmAngle_Page, ArmMotionSave);
                AddItem(" -Execute_Motions", &ExecuteMotions_Item, &ArmAngle_Page, &ArmAngle_Page, MotionRun);
                AddItem(" -Reset", &Reset_Item, &ArmAngle_Page, &ArmAngle_Page, ResetAngle);
                AddItem(" -Return", &ArmAngleReturn_Item, &ArmAngle_Page, &Home_Page, NULL);
        AddItem(" +Games", &Games_Item, &Home_Page, &Games_Page, NULL);
            AddPage("[Games]", &Games_Page);
                AddItem(" -Dino Game", &Dino_Item, &Games_Page, &Games_Page, NULL);
                AddItem(" -AirPlane Game", &AirPlane_Item, &Games_Page, &Games_Page, NULL);
                AddItem(" -Return", &GamesReturn_Item, &Games_Page, &Home_Page, NULL);
        AddItem(" -ShowLogo", &ShowLogo_Item, &Home_Page, &Home_Page, Show_Log);
        AddItem(" -Github", &Github_Item, &Home_Page, &Home_Page, Show_GitHub);
        AddItem(" -Bilibili", &Bilibili_Item, &Home_Page, &Home_Page, Show_Bilibili);
        AddItem(" -ReadME", &ReadME_Item, &Home_Page, &Home_Page, NULL);
}

uint8_t MENU_STATE = MENU_RUN;
uint8_t disapper = 1;
//初始化为开始菜单项
xpItem temp_item = &System_Item;
Itemfunction App_Function;

void Switch_Menu_State(uint8_t state)
{
    MENU_STATE = state;
}

uint8_t BtnScan(void)
{
    if(ButtonScan()==Up)return MENU_UP;
    else if(ButtonScan()==Down)return MENU_DOWN;
    else if(ButtonScan()==Center)return MENU_ENTER;
    return MENU_NONE;
}

void Process_Menu_Run(uint8_t Dir)
{
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
                if (temp_item->Itemfunction != NULL)
                {
                    ui_disapper(disapper);
                    App_Function = temp_item->Itemfunction;
                    Switch_Menu_State(APP_RUN);
                }
                else
                {
                    Switch_Menu_State(MENU_RUN);
                    for (size_t i = 0; i < 8; i++)
                    {
                        disapper = ui_disapper(disapper);
                    }
                    Draw_Menu(FirstPos, temp_item->JumpPage, Font_Size, temp_item, temp_item->JumpPage->itemHead);
                    temp_item = temp_item->JumpPage->itemHead;
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

void Process_App_Run(void)
{
    (*App_Function)();
}

void Menu_Task(void)
{
    uint8_t Dir = BtnScan();
    
    switch (MENU_STATE)
    {
        case MENU_RUN:
            Process_Menu_Run(Dir);
            break;
            
        case APP_RUN:
            Process_App_Run();
            break;
            
        case APP_BREAK:
            Process_Menu_Run(Dir);
            break;
            
        default:
            break;
    }
}

void Menu_Init(void)
{
    u8g2.begin();
    u8g2.enableUTF8Print();
    Menu_Team();
    ArmMotionInit();
    Draw_Process();
    //画出第一页菜单
    Draw_Menu(FirstPos, &Home_Page, Font_Size, &System_Item, &System_Item);
}