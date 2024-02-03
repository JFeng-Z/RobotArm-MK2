#ifndef __MENU_H__
#define __MENU_H__

#include "include.h"
#include "MenuConfig.h"

void Menu_Team(void);
void AddPage(const char *name, xpPage page);
void AddItem(const char *Name, Item_Type Type, int *Data, xpItem item, xpPage LocalPage, xpPage nextpage, Itemfunction function);
int8_t Line(uint8_t AllTime,uint8_t Time_Now,int8_t Tgt,int8_t Now);
uint8_t PID(int8_t Targrt, int8_t Now, Pid_Error *Obj);
void Draw_OptionPlace(uint8_t now_time, xpItem now_item, xpItem next_item);
void Draw_DialogBox(u8g2_uint_t x,u8g2_uint_t y,u8g2_uint_t w,u8g2_uint_t h);
void Draw_DialogRBox(u8g2_uint_t x,u8g2_uint_t y,u8g2_uint_t w,u8g2_uint_t h,u8g2_uint_t r);
void DialogScale_Show(uint8_t x,uint8_t y,uint8_t Targrt_w,uint8_t Targrt_h);
void Draw_Page(uint8_t pos, xpPage Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item);
void Draw_Menu(uint8_t pos, xpPage Page, uint8_t LineSpacing, xpItem now_item,xpItem next_item);
uint8_t ui_disapper(uint8_t disapper);
void Switch_Menu_State(Menu_State state);
Menu_State BtnScan(void);
void Menu_Task(void);
void Menu_Init(void);

#endif
