#include "SelectImgWin.h"
#include "BUTTON.h"
#include "ShowImage.h"
#include "common.h"
#include "ff.h"
#include "my_png.h"
#include "malloc.h"
static int status=0;
static WM_HWIN _hBut[4];
  uint16_t   curindex = 0 ;   /* 图片当前索引 */	
	
/**
 * @brief       背景窗口回调函数
 * @param       pMsg : WM窗口
 * @return      无
 */
static void _cbBkWin1(WM_MESSAGE *pMsg)
{
	uint32_t msg;
	
	    switch (pMsg->MsgId)
    {
	      case WM_CREATE:  		
			      GUI_SetBkColor(GUI_GRAY);
         //  GUI_SetFont(&GUI_FontHZ16);
           GUI_SetColor(GUI_RED);
           GUI_Clear();	
        case WM_PAINT:
					 GUI_SetBkColor(GUI_YELLOW);
         //  GUI_SetFont(&GUI_FontHZ16);
           GUI_SetColor(GUI_RED);
           GUI_Clear();		
      //     GUI_SetBkColor(GUI_BLUE);
      //   GUI_Clear();
       //     GUI_SetColor(0x0060FF);
        //    GUI_DispStringAt("PaintCount (Early):", 0, 0);

				
          //  GUI_DispDecAt(_PaintCount1, 120, 0, 5);
          //  GUI_SetColor(0x00FFC0);
          //  GUI_DispStringAt("PaintCount (Late):", 0, 12);
          //  GUI_DispDecAt(_PaintCount2, 120, 12, 5);
            break;
				
	        case WM_NOTIFY_PARENT:
            if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) /* 按钮被释放时 */
            {
                if (pMsg->hWinSrc == _hBut[0])              /* button1被释放 */
                {
									//SelectImage1(NULL,1);
									status=1;
									
									if(curindex>0)
									{
										curindex--;
									}										
									
									 WM_InvalidateWindow(pMsg->hWin);      /* 主窗口失效 */
                //    WM_InvalidateWindow(_hWin[0]);          /* 窗口1失效 */
                //    WM_InvalidateWindow(_hWin[1]);          /* 窗口2失效 */
                }
                else if (pMsg->hWinSrc == _hBut[1])         /* button2被释放 */
                {
                //    _PaintCount1 = 0;
                 //   _PaintCount2 = 0;
									status=2;
									if(curindex<info_PicFile.num_file-1)
									{
										
										curindex++;
									}									
                   // WM_InvalidateWindow(pMsg->hWin);      /* 主窗口失效 */
                }
		            else if (pMsg->hWinSrc == _hBut[2])         /* button2被释放 */
                {
					          status=3;
				
									
								//		WM_DeleteWindow(pMsg->hWin);
	
                //    _PaintCount1 = 0;
                 //   _PaintCount2 = 0;
         //           WM_InvalidateWindow(pMsg->hWin);      /* 主窗口失效 */
                }						
			          else if (pMsg->hWinSrc == _hBut[3])         /* button2被释放 */
                {
									 status=4;
                //    _PaintCount1 = 0;
                 //   _PaintCount2 = 0;
                  //  WM_InvalidateWindow(pMsg->hWin);      /* 主窗口失效 */
                }					
            }

            break;			
				
        default:
            WM_DefaultProc(pMsg);
    }	
}

void invokeSeleImgWin(void)
{
	uint32_t msg;
	int i=0;
	int res;
	DIR picdir;             /* 图片目录 */
	FILINFO dtfileinfo;
  FILINFO *tfileinfo=(FILINFO *)&dtfileinfo;    	
  char *pname;            /* 带路径的文件名 */	
	if(pname!=NULL)
		Mem_free(EXSRAM,pname);
  pname = Mem_malloc(EXSRAM, FF_MAX_LFN * 2 + 1);               /* 为带路径的文件名分配内存 */	
	WM_SetCallback(WM_HBKWIN, _cbBkWin1);	
	

	 _hBut[0] = BUTTON_CreateEx(10, 260, 44, 44, 0, WM_CF_SHOW, 0, GUI_ID_USER+10);    /* 创建按钮1 */
	 _hBut[1] = BUTTON_CreateEx(66, 260, 44, 44, 0, WM_CF_SHOW, 0, GUI_ID_USER+11);    /* 创建按钮2 */
	 _hBut[2] = BUTTON_CreateEx(122, 260, 44, 44, 0, WM_CF_SHOW, 0, GUI_ID_USER+12);    /* 创建按钮3 */
	 _hBut[3] = BUTTON_CreateEx(178, 260, 44, 44, 0, WM_CF_SHOW, 0, GUI_ID_USER+13);    /* 创建按钮4 */
	
	  BUTTON_SetText(_hBut[0], "Prev"); /* 设置按钮的名字 */
    BUTTON_SetText(_hBut[1], "Next");
		BUTTON_SetText(_hBut[2], "Cancel"); /* 设置按钮的名字 */
    BUTTON_SetText(_hBut[3], "OK");
	
	  for(i=0;i<4;i++)
	{
			 BUTTON_SetFocussable(_hBut[i],0);

	}

	  status=1;

	  res = f_opendir(&picdir, (const TCHAR *)pmyPicDir);              /* 打开目录 */
	

	
	
	  while (res == FR_OK)    /* 打开成功 */
    {
		     dir_sdi(&picdir, info_PicFile.picoffsettbl[curindex]);           /* 改变当前目录索引 */
         res = f_readdir(&picdir, (FILINFO*)tfileinfo);              /* 读取目录下的一个文件 */

        if (res != FR_OK || tfileinfo->fname[0] == 0)
				{
					Mem_free(EXSRAM,pname);
					  res=1;
           
					continue;          /* 错误了/到末尾了,退出 */			
				}
				
				sprintf(pname,"%s/%s",pmyPicDir,tfileinfo->fname);
				
		//		printf("pname=%s pmyPicDir=%s tfileinfo->fname=%s \r\n",pname,pmyPicDir,tfileinfo->fname);
	   //   strcpy((char *)pname, "0:/PICTURE/");                    /* 复制路径(目录) */
     //   strcat((char *)pname, (const char *)tfileinfo->fname); /* 将文件名接在后面 */		

				if(status==1)
					{
							GUI_SetBkColor(GUI_BLUE);
							GUI_ClearRect(0, 0, 239, 239);
			        printf("Prev curindex=%d pname=%s \r\n",curindex,pname);
					//	emwin_displaypng("0:/PICTURE/微软.png", 1, 200, 100);
						emwin_displaypng(pname, 0, 5, 5);
					//		emwin_displaypng(pname, 1, 200, 100);
        	//			  	SelectImage1(NULL,1);
							GUI_Delay(5);
							status=0;
//
		      }
	    	else if(status==2)
		    {
						GUI_ClearRect(0, 0, 239, 239);
					
					  printf("Prev curindex=%d pname=%s \r\n",curindex,pname);
		/*        if(curindex==5)
	  			     emwin_displaypng("0:/PICTURE/PNG/企鹅.png", 1, 200, 100);
						else*/
							emwin_displaypng(pname, 1, 200, 100);
        	//			  	SelectImage1(NULL,1);
		  		  status=0;
			      GUI_Delay(5);
			
		     }
		     else if(status==3)
		     {
						msg	= MSG_RETURN_MAIN;
						(void)osMessagePut(myQueue01Handle, msg, 0U);		
						GUI_SetBkColor(GUI_BLUE);
   // GUI_SetFont(&GUI_FontHZ16);
						GUI_SetColor(GUI_RED);
						GUI_Clear();
						for(i=0;i<4;i++)
						{
								WM_DeleteWindow(_hBut[i]);
								_hBut[i]=0;
						}
						status=0;
						Mem_free(EXSRAM,pname);
				    res=1;
					}
				else if(status==4)
				{
					   msg	= MSG_SETCW_PWD;
						(void)osMessagePut(myQueue01Handle, msg, 0U);		
						GUI_SetBkColor(GUI_RED);
   // GUI_SetFont(&GUI_FontHZ16);
						GUI_SetColor(GUI_RED);
						GUI_Clear();		
						for(i=0;i<4;i++)
						{
								WM_DeleteWindow(_hBut[i]);
								_hBut[i]=0;
						}
						status=0;
						Mem_free(EXSRAM,pname);
      			res=1;
			
	   	}
	  	else 
			 GUI_Delay(5);
	 }
				
			

		
}