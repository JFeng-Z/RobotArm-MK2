#ifndef __MENU_H__
#define __MENU_H__

#include <Arduino.h>
#include <U8g2lib.h>
#include "app.h"
#include "ArmControl.h"
extern "C"{
    #include "button.h"
}

//菜单字体
#define MENU_FONT   u8g2_font_profont12_mf
//起始Y坐标
#define FirstLine   9
//起始X坐标
#define FirstPos    0
//字体尺寸
#define Font_Size   12
//最大Y坐标
#define LINE_MAX    48
//最小Y坐标
#define LINE_MIN    12 
//最大可见项目数
#define MaxVisible_Number  4  

typedef struct Error
{
    float error;
    float sum_srror;
    float last_error;
}Error;

//菜单状态枚举
enum MenuState
{
    MENU_RUN,
    APP_RUN,
    APP_BREAK,
    MENU_MOVE,
    CURSOR_STATIC,

    MENU_UP,
    MENU_DOWN,
    MENU_ENTER,
    MENU_NONE
};

typedef void (*Itemfunction)(void);
typedef struct MenuPage *xpMenu;
typedef struct Item *xpItem;
typedef struct MenuPage
{
    const char *PageName;
    uint8_t len;
    xpItem ParentiTem;
    xpItem itemHead;
    xpItem itemTail;
} xMenu;

typedef struct Item
{
    const char *itemName;
    uint8_t id;
    xpMenu location, JumpPage;
    xpItem lastiTem, nextiTem;
    void(*Itemfunction)();
} xItem;

void Menu_Team(void);
void AddPage(const char *name, xpMenu page);
void AddItem(const char *Name, xpItem item, xpMenu LocalPage, xpMenu nextpage, Itemfunction function);
int8_t Line(uint8_t AllTime,uint8_t Time_Now,int8_t Tgt,int8_t Now);
uint8_t PID(int8_t Targrt, int8_t Now, Error *Obj);
void Draw_Process(void);
void Draw_OptionPlace(uint8_t now_time, xpItem now_item, xpItem next_item);
void Draw_DialogBox(u8g2_uint_t x,u8g2_uint_t y,u8g2_uint_t w,u8g2_uint_t h);
void Draw_DialogRBox(u8g2_uint_t x,u8g2_uint_t y,u8g2_uint_t w,u8g2_uint_t h,u8g2_uint_t r);
void DialogScale_Show(uint8_t x,uint8_t y,uint8_t Targrt_w,uint8_t Targrt_h);
void Draw_Page(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item);
void Draw_Menu(uint8_t pos, xpMenu Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item);
uint8_t ui_disapper(uint8_t disapper);
void Switch_Menu_State(uint8_t state);
uint8_t BtnScan(void);
void Menu_Task(void);
void Menu_Init(void);

#endif
