#include "showQrcode.h"
#include "BUTTON.h"
#include "my_qrCode.h"

static WM_HWIN _hBut_Return;

static void _cbBkWin_qr(WM_MESSAGE *pMsg)
{
		uint32_t msg;
	
	    switch (pMsg->MsgId)
    {
	      case WM_CREATE:  		
			      GUI_SetBkColor(GUI_GRAY);
         //  GUI_SetFont(&GUI_FontHZ16);
           GUI_SetColor(GUI_RED);
           GUI_Clear();	
			BUTTON_SetSkinClassic(_hBut_Return); 
    BUTTON_SetFont(_hBut_Return, GUI_FONT_16B_ASCII);
		BUTTON_SetTextColor(_hBut_Return, 0, GUI_BLUE);		
					show_qrcode();
				
				break;
        case WM_PAINT:
					 GUI_SetBkColor(GUI_YELLOW);
         //  GUI_SetFont(&GUI_FontHZ16);
           GUI_SetColor(GUI_RED);
           GUI_Clear();		
				BUTTON_SetSkinClassic(_hBut_Return); 
    BUTTON_SetFont(_hBut_Return, GUI_FONT_16B_ASCII);
		BUTTON_SetTextColor(_hBut_Return, 0, GUI_BLUE);				
									show_qrcode();
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
                if (pMsg->hWinSrc == _hBut_Return)              /* button1被释放 */
                {
									//SelectImage1(NULL,1);
											//	   WM_DeleteWindow(pMsg->hWin);
  			                msg	= MSG_RETURN_MAIN;
							//		show_qrcode();
								//	 WM_InvalidateWindow(pMsg->hWin);      /* 主窗口失效 */
                //    WM_InvalidateWindow(_hWin[0]);          /* 窗口1失效 */
                //    WM_InvalidateWindow(_hWin[1]);          /* 窗口2失效 */
												  WM_DeleteWindow(_hBut_Return);
				_hBut_Return=0;
									(void)osMessagePut(myQueue01Handle, msg, 0U);		
                }
		
            }

            break;			
				
        default:
            WM_DefaultProc(pMsg);			
				
				
				
			}
	
	
}
void invokeQrcode(void)
{
	uint32_t msg;
	int i=0;

	WM_SetCallback(WM_HBKWIN, _cbBkWin_qr);	

	_hBut_Return = BUTTON_CreateEx(60, 260, 110, 30, 0, WM_CF_SHOW, 0, GUI_ID_USER+20);    /* 创建按钮*/
   BUTTON_SetFont(_hBut_Return, GUI_FONT_16B_ASCII);
		BUTTON_SetTextColor(_hBut_Return, 0, GUI_BLUE);
	    BUTTON_SetText(_hBut_Return, "Return");
	
	
}