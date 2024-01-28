#include "my_qrCode.h"
#include "GUI_Type.h"
#include "GUI.h"

#include "string.h"
GUI_HMEM QR_CODE;

void show_qrcode(void)
{
	    GUI_SetBkColor(GUI_BLUE);
   // GUI_SetFont(&GUI_FontHZ16);
 //   GUI_SetColor(GUI_RED);
  //  GUI_Clear();

	    int xSize;
    int ySize;
	    xSize   = LCD_GetXSize();
    ySize   = LCD_GetYSize();
	
	printf("GUI_QR_Create x=%d y=%d\r\n",xSize,ySize);
	 QR_CODE = GUI_QR_Create("abxsd", 5, GUI_QR_ECLEVEL_Q, 0);
    GUI_QR_Draw(QR_CODE, 10, 10);//显示二维码
	  GUI_QR_Draw(QR_CODE, 130, 10);//显示二维码
	  GUI_QR_Draw(QR_CODE, 10, 130);//显示二维码
	  GUI_QR_Draw(QR_CODE, 130,130);//显示二维码
	
    GUI_QR_Delete(QR_CODE);//释放存储二维码的内存空间
	
	
	
}

/*void ShowQRcode(char *str)
{
    QRcode *qrcode;
    unsigned short x,y;
    unsigned short multiple;
    unsigned short xoffset, yoffset;
    unsigned short xpos, ypos;
    unsigned short qrsize = 150;
    int xSize;
    int ySize;


    xSize   = LCD_GetXSize();
    ySize   = LCD_GetYSize();
    xoffset = (xSize - qrsize ) / 2;
    yoffset = (ySize - qrsize ) / 2;
    qrcode = QRcode_encodeString(str, 2, QR_ECLEVEL_Q, QR_MODE_8, 0);

    if(qrcode != NULL)
    {
        xpos = (xSize - qrsize ) / 2 - 15;
        ypos = (ySize - qrsize ) / 2 - 15;
        GUI_SetColor(GUI_WHITE);
        GUI_FillRect(xpos, ypos, xpos + qrsize + 30, ypos + qrsize + 30);
        multiple = qrsize / qrcode->width;
        GUI_SetColor(GUI_BLACK);
        for(y=0;y<qrcode->width;y++)
        {
            for(x=0;x<qrcode->width;x++)
            {
                if(qrcode->data[y*qrcode->width+x]&0x01)
                {
                    xpos = multiple*x + xoffset;
                    ypos = y*multiple + yoffset;
                    GUI_FillRect(xpos, ypos, xpos + multiple, ypos + multiple);
                }
            }
        }
        QRcode_free(qrcode);
    } 
}*/


   /* void MainTask(void) {
        GUI_HMEM QR_CODE;
        GUI_Init();
        if (GUI_ALLOC_GetNumFreeBytes() < RECOMMENDED_MEMORY) {
            GUI_ErrorOut("Not enough memory available.");
            return;
        }
        GUI_SetBkColor(GUI_DARKGRAY);
        GUI_Clear();
        //生成QR_CODE代码的句柄
        QR_CODE = GUI_QR_Create("abxsd", 8, GUI_QR_ECLEVEL_Q, 0);
        GUI_QR_Draw(QR_CODE, 30, 30);//显示二维码
        GUI_QR_Delete(QR_CODE);//释放存储二维码的内存空间
        while (1)
        {
        GUI_Delay(100);
        }
    }*/