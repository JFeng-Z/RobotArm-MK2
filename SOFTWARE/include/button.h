#ifndef _button_H_
#define _button_H_

#include "Arduino.h"

typedef enum BtnEnum
{
    Up,
    Down,
    Center,
    Lift,
    Right,
    NONE_PRESS
}BtnState;

BtnState ButtonScan(void);

#endif