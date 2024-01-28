#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "common.h"

void Touch_Init(void);
void Clear_Screen(void);
void Draw_Point(uint16_t x,uint16_t y,uint16_t color);	//在按下处画一个2*2点		
void R_Touch_test(void);

#endif

