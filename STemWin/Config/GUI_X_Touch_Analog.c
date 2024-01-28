/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.16 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/
#include "GUI.h"
#include "xpt2046.h"
#include "my_lcd.h"



void GUI_TOUCH_X_ActivateX(void) 
{

}


void GUI_TOUCH_X_ActivateY(void)
{

}

int  GUI_TOUCH_X_MeasureX(void) 
{
	uint8_t touch_buf[4];
	uint16_t Cx,Cy;
		
	if(lcd_id==0x9341)
	{
      Cx=RTouch_Read_XorY(0xD0);  //CMD_RDX=0XD0
	//	  printf(" RTouch_Read_XorY Cx=%d \r\n",Cx);
		  return  Cx;
	}
	
	else if(lcd_id==0x1963)
	  {
		   // Read_Touch_Point(0x03,touch_buf);

				if(dir_flag==1)//∫·∆¡
					{
						Cx=((uint16_t)(touch_buf[0]&0X0F)<<8)+touch_buf[1];
						Cy=((uint16_t)(touch_buf[2]&0X0F)<<8)+touch_buf[3];						
					}else        // ˙∆¡
					{
						Cx=480-(((uint16_t)(touch_buf[2]&0X0F)<<8)+touch_buf[3]);
						Cy=((uint16_t)(touch_buf[0]&0X0F)<<8)+touch_buf[1];
					}
			return	Cx;	
		}
}


int  GUI_TOUCH_X_MeasureY(void) 
{	
		uint8_t touch_buf[4];
	uint16_t Cx,Cy;
	
	if(lcd_id==0x9341)
	{
     Cy = RTouch_Read_XorY(0x90);  //CMD_RDY=0XD0
		 return	Cy;	
		//  printf(" RTouch_Read_XorY Cy=%d \r\n",Cy);
	}
		
	else if(lcd_id==0x1963)
	  {
		  //  Read_Touch_Point(0x03,touch_buf);

				if(dir_flag==1)//∫·∆¡
					{
						Cx=((uint16_t)(touch_buf[0]&0X0F)<<8)+touch_buf[1];
						Cy=((uint16_t)(touch_buf[2]&0X0F)<<8)+touch_buf[3];						
					}else        // ˙∆¡
					{
						Cx=480-(((uint16_t)(touch_buf[2]&0X0F)<<8)+touch_buf[3]);
						Cy=((uint16_t)(touch_buf[0]&0X0F)<<8)+touch_buf[1];
					}
			return	Cy;	
		}
}

