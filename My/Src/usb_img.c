#include "usb_img.h"
#include "ff.h"
#include "exfuns.h"
#include "usb_host.h"
#include "fatfs.h"
#include "my_lcd.h"
#include "malloc.h"
#include "piclib.h"
#define FF_USE_LFN		3
#define FF_MAX_LFN		255
//cwpro\cwpro.axf: Error: L6218E: Undefined symbol exfuns_file_type (referred from usb_img.o).
//cwpro\cwpro.axf: Error: L6218E: Undefined symbol g_flag_mount (referred from usb_img.o).
//cwpro\cwpro.axf: Error: L6218E: Undefined symbol piclib_ai_load_picfile (referred from usb_img.o).
//cwpro\cwpro.axf: Error: L6218E: Undefined symbol piclib_init (referred from usb_img.o).
	void lcdbenin(void);
uint16_t pic_get_tnum(char *path)
{
    uint8_t res;
    uint16_t rval = 0;
    DIR tdir;                                                 /* ��ʱĿ¼ */
	 FILINFO dtfileinfo;
	
    FILINFO *tfileinfo=(FILINFO *)&dtfileinfo;                                       /* ��ʱ�ļ���Ϣ */

  //  tfileinfo = (FILINFO *)Mem_malloc(INSRAM, sizeof(FILINFO)); /* �����ڴ� */
	//  goto  _end_;

    res = f_opendir(&tdir, (const TCHAR *)path);              /* ��Ŀ¼ */

    if (res == FR_OK && tfileinfo)
    {
        while (1)                               /* ��ѯ�ܵ���Ч�ļ��� */
        {
            res = f_readdir(&tdir, tfileinfo);  /* ��ȡĿ¼�µ�һ���ļ� */

            if (res != FR_OK || tfileinfo->fname[0] == 0)
                break;                          /* ������/��ĩβ��,�˳� */

            res = exfuns_file_type(tfileinfo->fname);

            if ((res & 0XF0) == 0X50)           /* ȡ����λ,�����ǲ���ͼƬ�ļ� */
            {
                rval++;                         /* ��Ч�ļ�������1 */
            }
        }
    }

//		_end_:		    Mem_free(INSRAM, tfileinfo);                  /* �ͷ��ڴ� */
        return rval;
}

int USBDiskTest(void)
{
	static ApplicationTypeDef lastAppliState = APPLICATION_IDLE;
		int flag_app = 0;
	if (GetUsbAppliState() != lastAppliState)
		{
						if (GetUsbAppliState() == APPLICATION_READY)
			{
				flag_app=1;
			}
			
		}			
		
		return flag_app;
}
int USBDiskTest1(void)
{
static ApplicationTypeDef lastAppliState = APPLICATION_IDLE;
	UINT br, bw;
	int flag_app = 0;
	
	DIR picdir;             /* ͼƬĿ¼ */
	    uint16_t totpicnum;     /* ͼƬ�ļ����� */
	uint8_t *pname;
	FILINFO *picfileinfo;   /* �ļ���Ϣ */
    uint16_t curindex;      /* ͼƬ��ǰ���� */	
    uint32_t *picoffsettbl; /* ͼƬ�ļ�offset������ */

    uint16_t temp;	
	if (GetUsbAppliState() != lastAppliState)
		{
			if (GetUsbAppliState() == APPLICATION_READY)
			{
				flag_app=1;
				
				int res = f_mount(&USBHFatFS,USBHPath,1);//����U��
												 //USBHFatFS��USBHPath����������ϵͳ��fatfs.c�����Ѿ�������ˣ�ֱ����
				if(res == FR_OK)
				{
					printf("f_mount SUCCESSFUL: %s\r\n",USBHPath);
//					g_flag_mount=1;
				}
				else
				{
					printf("f_mount ERROR: %d\r\n",res);
				}
				
				res = f_opendir(&picdir, "0:/");
				if(res==FR_OK)
				{
					printf("f_opendir SUCCESSFUL 0:/ddd\r\n");
				}
				 totpicnum = pic_get_tnum("0:/");     /* �õ�����Ч�ļ��� */
				
				printf("000totpicnum = %d\r\n",totpicnum);
				
	      picoffsettbl =(uint32_t *) Mem_malloc(EXSRAM,4 * totpicnum);             /* ����4*totpicnum���ֽڵ��ڴ�,���ڴ��ͼƬ���� */	
				if(!picoffsettbl)
				   				printf("malloc picoffsettbl err err !!!\r\n");
				else
					printf("111malloc picoffsettbl succ !!!\r\n");
			

				
				
				pname =(uint8_t *) malloc(FF_MAX_LFN * 2 + 1);            /* Ϊ��·�����ļ��������ڴ� */
				
				if(!pname)
					printf("malloc pname err err !!!\r\n");
				else
					printf("malloc pname succ !!!\r\n");
				
				
				
				picfileinfo = (FILINFO *)malloc(sizeof(FILINFO)); /* �����ڴ� */
				
				if(!picfileinfo)
				   				printf("malloc picfileinfo err err !!!\r\n");
				
				
    /* ��¼���� */
    res = f_opendir(&picdir, "0:/PICTURE");         /* ��Ŀ¼ */

    if (res == FR_OK)
    {
        curindex = 0;   /* ��ǰ����Ϊ0 */

        while (1)       /* ȫ����ѯһ�� */
        {
            temp = picdir.dptr;                     /* ��¼��ǰdptrƫ�� */
            res = f_readdir(&picdir, picfileinfo);  /* ��ȡĿ¼�µ�һ���ļ� */

            if (res != FR_OK || picfileinfo->fname[0] == 0)
                break;  /* ������/��ĩβ��,�˳� */

            res = 0;//exfuns_file_type(picfileinfo->fname);

            if ((res & 0XF0) == 0X50)               /* ȡ����λ,�����ǲ���ͼƬ�ļ� */
            {
                picoffsettbl[curindex] = temp;      /* ��¼���� */
                curindex++;
            }
        
					}
				}
		
	      LCD_DisplayString(20,190,16,RED,(uint8_t *)"show a picture");	
				
	      osDelay(1500);
//        piclib_init();                                          /* ��ʼ����ͼ */
        curindex = 0;                                           /* ��0��ʼ��ʾ */
        res = f_opendir(&picdir, (const TCHAR *)"0:/PICTURE");  /* ��Ŀ¼ */

    if (res == FR_OK)    /* �򿪳ɹ� */
    {
        dir_sdi(&picdir, picoffsettbl[curindex]);           /* �ı䵱ǰĿ¼���� */		
	        res = f_readdir(&picdir, picfileinfo);              /* ��ȡĿ¼�µ�һ���ļ� */

        if (res != FR_OK || picfileinfo->fname[0] == 0)
					printf("xxxxxxxxxxxxxx\r\n");
//            break;          /* ������/��ĩβ��,�˳� */		
	        strcpy((char *)pname, "0:/PICTURE/");                    /* ����·��(Ŀ¼) */
          strcat((char *)pname, (const char *)picfileinfo->fname); /* ���ļ������ں��� */		
		 //     LCD_Clear(BLACK);	
//		      res = piclib_ai_load_picfile((char*)pname, 0, 0, lcd_width, lcd_height, 1); /* ��ʾͼƬ */
          printf("piclib_ai_load_picfile  res=0x%x\r\n",res);
				
				
				
		}			
				free(pname);
				free(picfileinfo);
				free(picoffsettbl);
				
/*				res = f_open(&USBHFile,"0:/PICTURE/config.bmp",FA_READ);//���ļ�
		    printf("xxxxxxxxxxxxxxxxxxx\r\n");
				if(res == FR_OK)
				{
					printf("f_open SUCCESSFUL 0:/PICTURE/config.bmp\r\n");
					char buffer[10] = { 0 };
					res = f_read(&USBHFile,buffer,10,&br);
					if (br == 10)
						printf("f_read buffer = %s \r\n",buffer);
					else
							printf("f_read buffer %s len =%d \r\n",buffer,br);
					f_close(&USBHFile);	
				}
				else
				{
					printf("!!!!!!!!f_open ERROR: %d\r\n",res);
				}*/
				
				res = f_open(&USBHFile, "0:/config.txt", FA_WRITE | FA_OPEN_APPEND);
				if (res == FR_OK)
				{
					printf("f_open SUCCESSFUL 0:/config.txt\r\n");
					UINT len = strlen("Hello\n");
					f_write(&USBHFile, "Hello\n", len, &bw);
					if (len == bw)
						printf("f_write success \r\n");
					f_close(&USBHFile);
				}
				else
				{
					printf("f_open ERROR: %d\r\n",res);
				}
			}
			lastAppliState = GetUsbAppliState();
		}
	  return flag_app;
}