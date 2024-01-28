/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "usart.h"
#include "key_led.h"
#include "my_lcd.h"
#include "touch.h"
#include "xpt2046.h"
#include "GUI.h"
#include "MainDLG.h"
#include "24c02.h"
#include "ff.h"
#include "my_png.h"
#include "SelectImgWin.h"
#include "enterImgPwd.h"
#include "resultDLG.h"
//#include "enterImgPwd.h"

//#include "FramewinDLG.h"

//#include "emwin_demo.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
FATFS fsTF;  //TF卡文件系统	

void lcdbenin(void);
void emwin_main(void);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
 
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId Task_touchHandle;
osThreadId Task_mainHandle;
osMessageQId myQueue01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask_touch(void const * argument);
void StartTask_main(void const * argument);

extern void MX_USB_HOST_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of myQueue01 */
  osMessageQDef(myQueue01, 16, uint16_t);
  myQueue01Handle = osMessageCreate(osMessageQ(myQueue01), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 1024);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Task_touch */
  osThreadDef(Task_touch, StartTask_touch, osPriorityLow, 0, 1024);
  Task_touchHandle = osThreadCreate(osThread(Task_touch), NULL);

  /* definition and creation of Task_main */
  osThreadDef(Task_main, StartTask_main, osPriorityNormal, 0, 2048);
  Task_mainHandle = osThreadCreate(osThread(Task_main), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	

	
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_HOST */
  MX_USB_HOST_Init();
  /* USER CODE BEGIN StartDefaultTask */

  /* Infinite loop */
  for(;;)
  {
		


		
		
		osDelay(2000);
 //   GUI_Delay(1000);
//		AT24C02_Read(0,(uint8_t *)xx,sizeof(xx));
		
		printf("yyyyyyyyyyyyyyyyyy\r\n");
		
//		if(g_sys_status==SYS_CREATE_WALLET)
			
//			emwin_main();
		
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask_touch */
/**
* @brief Function implementing the Task_touch thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_touch */
void StartTask_touch(void const * argument)
{
  /* USER CODE BEGIN StartTask_touch */
	int i=0;
   uint16_t xx=0;
	uint16_t yy=0;
		Touch_Init();
	//    WM_MESSAGE message;
	
	printf("000StartTask02 StartTask02\r\n");
  //  GUI_CURSOR_Show();	
	
  /* Infinite loop */
  for(;;)
  {
  //    xx = RTouch_Read_XorY(0xD0);
	//    yy = RTouch_Read_XorY(0x90); 
	//	  printf("RTouch_Read_XorY  xx=%d  yy=%d\r\n",xx,yy);
		    i++;
		   if(i==1000)
			 {
				 i=0;
  //  message.MsgId = MSG_CUSTOM;
				 printf("StartTask02 StartTask02 \r\n");
			//	 WM_SendMessage(g_hWin, &message);
				 
				 
			 }
        GUI_TOUCH_Exec();
		  	//	osDelay(5);
        GUI_Delay(5);
  }
  /* USER CODE END StartTask_touch */
}

/* USER CODE BEGIN Header_StartTask_main */
/**
* @brief Function implementing the Task_main thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_main */
void StartTask_main(void const * argument)
{
  /* USER CODE BEGIN StartTask_main */
		int ret=0xff;	

	
	
	char xx[]="abcdef";
		LCD_Init();
  //	lcdbenin();
	  GUI_Init();
	  LED1(1);
	  LED2(1);
		LED3(1);
	  printf("GUI_Init succ!!!\r\n");

/*	GUI_SetBkColor(GUI_BLUE); //设置背景颜色
  GUI_SetColor(GUI_YELLOW); //设置颜色
  GUI_Clear(); //清屏
  GUI_SetFont(&GUI_Font24_ASCII); //设置字体
   GUI_DispStringAt("Hello Word!",0,0); */
//emwin_main();
  // GUIDEMO_Main();
	
//	GUIDEMO_Automotive();
//	AT24C02_Write(0,(uint8_t *)xx,sizeof(xx));
//	printf("111yyyyyyyyyyyyyyy\r\n");
	//printf("AT24C02_Read xx=%s \r\n",xx);
 //  emwin_main();
  //CreateMain();
	
//   Fatfs_RW_test();

	//CreateFramewin();
	//osDelay(2000);

		BSP_SD_Init();
	  osDelay(30);	
//	while(ret!=0)
	{
    ret = f_mount(&fsTF,"0:",1); 		//挂载TF卡 
		osDelay(50);
	}
//		osDelay(50);	
	  printf("\r\n Mounting the TF card successfully. ret=%d\r\n",(int)ret);	
	  main_menu();
//	CreateMain();
  // emwin_main();
  // emwin_main();
	  osEvent event;
	uint32_t msg;
	
  /* Infinite loop */
  for(;;)
  {
		event = osMessageGet(myQueue01Handle, osWaitForever);
    if (event.status == osEventMessage)
    {
			msg=event.value.v;
			printf("rev msg = %d \r\n",msg);
			if(msg==MSG_CREATE_WALLET)
			   invokeSeleImgWin();
			else if(msg==MSG_RETURN_MAIN)
			{
				main_menu();
			}
			else if(msg==MSG_SETCW_PWD)
			{
				
				enter_ImgPwdPad();
				//enter_ImgPwdPad();
			}		
			else if(msg==MSG_CW_RESULT)
			{
				CreateResDlg();
				
			}
				else if(msg==MSG_SHOW_QRCODE)
			{
				CreateResDlg();
				
			}		
   //   USBH_Process((USBH_HandleTypeDef *)argument);
    }
  }
  /* USER CODE END StartTask_main */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void lcdbenin(void)
{

	BRUSH_COLOR=RED;      //设置画笔颜色为红色
	LCD_DisplayString(0,10,16,BLACK,"Welcome to use secure wallet");//
	LCD_DisplayString(20,50,16,BLUE,"Create wallet");
	LCD_DisplayString(20,90,16,BLUE,"View wallet");
	LCD_DisplayString(20,130,16,RED,"Decrypt wallet");	

	
  LCD_DisplayString(lcd_width-40,lcd_height-18,16,RED,"OK");//显示清屏区域
	
//	LCD_DisplayString(50,130,16,"KEY1:Number Test");	
//	- Create wallet
//- View wallet
//- Decrypt wallet
}






/* USER CODE END Application */
