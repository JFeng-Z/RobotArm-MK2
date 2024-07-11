#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "include.h"
#include "MenuConfig.h"

void Show_MPU6050(xpMenu Menu);
void Show_GitHub(xpMenu Menu);
void Show_Bilibili(xpMenu Menu);
void Show_Log(xpMenu Menu);
void Menu_to_Home(void *value);
void ArmMotionSave(xpMenu Menu);
void Move(xpMenu Menu);
void Execute_Motions(xpMenu Menu);

#endif