#include "usb.h"

#include "fatfs.h"
/* USB通讯数据处理 */
	/*void my_USBH_UserProcess(USBH_HandleTypeDef *phost,uint8_t id)//ApplicationTypeDef Appli_state
{

    switch(id)
    {
        case APPLICATION_READY:
            MSC_Application();
          //  id = APPLICATION_DISCONNECT;
			
            break;
        
        case APPLICATION_DISCONNECT:
            f_mount(NULL, (const TCHAR*)"",0);
						 osDelay(10);
            break;
        
        default:
            break;
    }
		
	}

void my_USBH_UserProcess(USBH_HandleTypeDef *phost,ApplicationTypeDef Appli_state)
{
        static int status = 0;  
         if(Appli_state == APPLICATION_READY)//U盘已经加载完成
        {
					printf("99999 ssdlh\n");
		 			  if(status==0)
						{
							status=1;
							retUSBH=f_mount(&USBHFatFS, (TCHAR const*)USBHPath, 0);
							if(retUSBH==FR_OK)
							{
								printf("USB drive mounted successfully!\r\n");
								
								osDelay(1);
							}		
						}
						
						
 	       retUSBH = f_open(&USBHFile,(const char*)"测试.txt",FA_CREATE_ALWAYS|FA_WRITE);
            if(retUSBH==FR_OK)
            {
              printf("open file\"test.txt\"successfully!\r\n");
            }		
	          retUSBH = f_close(&USBHFile);			
							{
								printf("close file successfully!\r\n\r\n");
								
							}
          if(status == 0)//U盘加载后只运行一次
          {
            status = 1;
            retUSBH=f_mount(&USBHFatFS, (TCHAR const*)USBHPath, 1);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"挂载U盘成功!\r\n",14,100);
            }
            //写入文件测试
            HAL_UART_Transmit(&huart1,(uint8_t*)"写入文件测试!\r\n",15,100);
            retUSBH = f_open(&USBHFile,(const char*)"测试.txt",FA_CREATE_ALWAYS|FA_WRITE);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"打开文件\"测试.txt\"成功!\r\n",25,100);
            }
			
			retUSBH = f_lseek(&USBHFile,f_size(&USBHFile));
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"指针移动到文件末尾成功！\r\n",25,100);
            }
			
        //    retUSBH = f_write(&USBHFile,FileBuf,18,&bw);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"写入文件成功!\r\n",15,100);
            }
            retUSBH = f_close(&USBHFile);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"关闭文件成功!\r\n\r\n",17,100);
            }
            //读取文件测试
            HAL_UART_Transmit(&huart1,(uint8_t*)"读取文件测试!\r\n",15,100);
            retUSBH = f_open(&USBHFile,(const char*)"测试.txt",FA_OPEN_EXISTING|FA_READ);//打开文件
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"打开文件\"测试.txt\"成功!\r\n",25,100);
            }
        //    retUSBH = f_read(&USBHFile,FileBuf,18,&bw);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"读取文件成功!文件内容：",23,100);
       //       HAL_UART_Transmit(&huart1,FileBuf,bw,100);
            }
            retUSBH = f_close(&USBHFile);
            if(retUSBH==FR_OK)
            {
              HAL_UART_Transmit(&huart1,(uint8_t*)"关闭文件成功!\r\n",15,100);
			}
		}
	  else if(Appli_state == APPLICATION_IDLE)//U盘未插入
	  {
		   //等待下次U盘插入则重新写入
	  }
		else if(Appli_state ==APPLICATION_DISCONNECT)
		{
			
	//	   	retUSBH=f_mount(NULL, (TCHAR const*)USBHPath, 1);
				//						if(retUSBH==FR_OK)
							{
								printf("123 USB drive mounted successfully!\r\n");
							}		
			
			
           retUSBH = f_close(&USBHFile);
            if(retUSBH==FR_OK)
            {
              printf("关闭文件成功!\r\n");
						}
			
		}
	
	
}*/

