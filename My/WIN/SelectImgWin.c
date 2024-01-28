#include "SelectImgWin.h"
#include "BUTTON.h"
#include "ShowImage.h"
#include "common.h"
static int status=0;
static WM_HWIN _hBut[4];

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

									 WM_InvalidateWindow(pMsg->hWin);      /* 主窗口失效 */
                //    WM_InvalidateWindow(_hWin[0]);          /* 窗口1失效 */
                //    WM_InvalidateWindow(_hWin[1]);          /* 窗口2失效 */
                }
                else if (pMsg->hWinSrc == _hBut[1])         /* button2被释放 */
                {
                //    _PaintCount1 = 0;
                 //   _PaintCount2 = 0;
									status=2;

									
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
		//		  emwin_displaypng("0:/PICTURE/PNG/皇冠.png", 1, 200, 100);
	 // 	GUI_Delay(5);
	  status=1;

	while(1)
	{
	
		if(status==1)
		{
			GUI_SetBkColor(GUI_BLUE);
			  GUI_ClearRect(0, 0, 239, 239);
			
			  emwin_displaypng("0:/PICTURE/PNG/皇冠.png", 1, 200, 100);
        	//			  	SelectImage1(NULL,1);
		  	GUI_Delay(5);
			status=0;
//
		}
		else if(status==2)
		{
			  GUI_ClearRect(0, 0, 239, 239);
			
			  emwin_displaypng("0:/PICTURE/PNG/香蕉.png", 1, 200, 100);
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
			break;
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

			break;
			
		}
		else 
			 GUI_Delay(5);
	}
		
}