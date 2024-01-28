#ifndef __COMMON_H
#define __COMMON_H
#include "main.h"
#define _MYOS_

#ifdef _MYOS_
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#endif


typedef enum
{
  SYS_IDLE = 0U,
  /*SYS_CREATE_WALLET=10,
	SYS_CREATE_WALLET_NO=11,
	SYS_VIEW_WALLET=20,
	SYS_DECRYPT_WALLET=30,	*/
} SYS_StateTypeDef;
typedef enum
{
  MSG_IDLE = 0U,
  MSG_CREATE_WALLET=10,
	MSG_CREATE_WALLET_NO=11,
	MSG_VIEW_WALLET=20,
	MSG_DECRYPT_WALLET=30,	
	MSG_RETURN_MAIN=50,
	MSG_SETCW_PWD = 60,
  MSG_CW_RESULT = 70,
	MSG_SHOW_QRCODE = 80,
} MSG_StateTypeDef;

enum DiskStatus {
    NO_DISK_INSERTED,
    SD_CARD_INSERTED,
	  SD_CARD_FILES_READ,
    SD_CARD_EMPTY,
	   USB_INSERTED,
	  USB_FILES_READ,
    USB_EMPTY,
	
};
void delay_us(uint32_t nus);
#ifndef _MYOS_
void osDelay(uint32_t ms);
#else
extern osMessageQId myQueue01Handle;
#endif
#endif

