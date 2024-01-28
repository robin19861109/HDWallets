#ifndef __KEY_H
#define __KEY_H
#include "main.h"
#define key_0 HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_6)
#define key_1 HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_7)
#define key_2 HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_8)
#define key_3 HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_9)


#define	LED1(x) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,x) 
#define	LED2(x) HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,x) 
#define	LED3(x) HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,x) 


/* LED取反定义 */
#define LED1_TOGGLE()    do{ HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3); }while(0)       /* LED0 = !LED0 */
#define LED2_TOGGLE()    do{ HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4); }while(0)       /* LED0 = !LED0 */
#define LED3_TOGGLE()    do{ HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_9); }while(0)       /* LED0 = !LED0 */

/* 蜂鸣器控制 */
#define BEEP(x)       HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,x) 
	

/* BEEP状态翻转 */
#define BEEP_TOGGLE()   do{ HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_7); }while(0)    /* BEEP = !BEEP */

uint8_t my_key(void);
#endif
