/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.44 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf_FlexColor_Template.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *                      http://www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "GUI.h"
#include "GUIDRV_FlexColor.h"
#include "my_lcd.h"
/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/

//
// Physical display size
//
//与触摸屏有关定义，根据实际情况填写

#define TOUCH_AD_TOP		160  	//按下触摸屏的顶部，写下 Y 轴模拟输入值。
#define TOUCH_AD_BOTTOM		3990 	//按下触摸屏的底部，写下 Y 轴模拟输入值。
#define TOUCH_AD_LEFT 		160		//按下触摸屏的左侧，写下 X 轴模拟输入值。
#define TOUCH_AD_RIGHT		3990	//按下触摸屏的右侧，写下 X 轴模拟输入值。

/*
//屏幕大小
#define XSIZE_PHYS  240 //X轴
#define YSIZE_PHYS  320 //Y轴
#define VXSIZE_PHYS	240
#define VYSIZE_PHYS 320*/

#define XSIZE_PHYS  240 // To be adapted to x-screen size
#define YSIZE_PHYS  320 // To be adapted to y-screen size

//
// Color conversion
//   The color conversion functions should be selected according to
//   the color mode of the target display. Detaileds can be found in
//   the chapter "Colors" in the emWin user manual.
//
#define COLOR_CONVERSION GUICC_565

//
// Display driver
//   GUIDRV_WIN32 is for use only within the emWin Simulation
//   environment. In order to use the target display controller, the
//   according emWin display driver should be configured as it is
//   described in the chapter "Display Drivers" in the emWin user
//   manual. Beyond that sample configuration files can be found in
//   The folder "Sample\LCDConf\%DISPLAY_DRIVER%\".
//
#define DISPLAY_DRIVER GUIDRV_WIN32

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif
#ifndef   DISPLAY_DRIVER
  #error No display driver defined!
#endif
/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/


//配置程序,用于创建显示驱动器件,设置颜色转换程序和显示尺寸
//配置程序,用于创建显示驱动器件,设置颜色转换程序和显示尺寸
void LCD_X_Config(void) {
	
	GUI_DEVICE_CreateAndLink(&GUIDRV_Template_API, COLOR_CONVERSION, 0, 0); //创建显示驱动器件

		LCD_SetSizeEx    (0, lcd_width, lcd_height);
		LCD_SetVSizeEx   (0, lcd_width, lcd_height);
		if(lcd_id == 0x9341)
	    {
				if(dir_flag == 0) //竖屏
						{
							GUI_TOUCH_Calibrate(GUI_COORD_X,0,lcd_width,155,3903);
							GUI_TOUCH_Calibrate(GUI_COORD_Y,0,lcd_height,188,3935);
						}else //横屏
						{
							GUI_TOUCH_SetOrientation(GUI_SWAP_XY|GUI_MIRROR_Y); 
							GUI_TOUCH_Calibrate(GUI_COORD_X,0,240,155,3903); 	
							GUI_TOUCH_Calibrate(GUI_COORD_Y,0,320,188,3935);
						}
		   }
		 else if(lcd_id == 0x1963)
	     {
						if(dir_flag == 0) //竖屏
						{					
							GUI_TOUCH_Calibrate(GUI_COORD_X,0,800,0,799); 		
							GUI_TOUCH_Calibrate(GUI_COORD_Y,0,480,0,479);
						}else   //横屏
						{
							GUI_TOUCH_Calibrate(GUI_COORD_X,0,800,0,799); 		
							GUI_TOUCH_Calibrate(GUI_COORD_Y,0,480,0,479);
						}
	     }
}
//显示器驱动的回调函数


int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData)
{
	printf("LCD_X_Display\r\n");
 // GUI_USE_PARA(LayerIndex);
 //  GUI_USE_PARA(Cmd);
 //GUI_USE_PARA(pData);
  return 0;
}


/*************************** End of file ****************************/

