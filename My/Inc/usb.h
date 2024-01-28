#ifndef __USB_H
#define __USB_H
#include "main.h"
#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "cmsis_os.h"                  // ::CMSIS:RTOS2
#include "ff.h"


//void my_USBH_UserProcess(USBH_HandleTypeDef *phost,ApplicationTypeDef Appli_state);
ApplicationTypeDef GetUsbAppliState(void);
void my_USBH_UserProcess(USBH_HandleTypeDef *phost,uint8_t id);
FRESULT dir_sdi (	/* FR_OK(0):succeeded, !=0:error */
	DIR* dp,		/* Pointer to directory object */
	DWORD ofs		/* Offset of directory table */
);
#endif
