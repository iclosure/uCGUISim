/****************************************Copyright (c)**************************************************
**                               广州微嵌计算机科技有限公司
**
**                                 http://www.wqlcd.com
**
**------------------------------------------------------------------------------------------------------
** 创建人: 	陈华牧
** 版  本: 	V1.0
** 创建日期:2007年1月18日
** 发布日期:2007年3月15日
********************************************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "GUI.h"
#include "FRAMEWIN.h"
#include "MULTIPAGE.h"
#include "EDIT.H"
#include "FRAMEWIN.h"
#include "BUTTON.H"
#include "TEXT.H"
#include "RADIO.H"
#include "SLIDER.H"
#include "DROPDOWN.H"
#include "MULTIEDIT.H"


#define	 WM_MESSAGE_DISPCOMSTATE	WM_USER+0	//消息类型--显示串口状态


#define  GUI_ID_ButtonOk			0x814
#define  GUI_ID_ButtonOk1			0x815	//(由于有窗口弹出,增加一个ID作为临时ID)
#define	 GUI_ID_ButtonCancel		0x816
#define	 GUI_ID_ButtonCancel1		0x817
#define  GUI_ID_TouchCalibrate		0x81b
#define  GUI_ID_ButtonExit			0x81c

static const GUI_WIDGET_CREATE_INFO _aMainFrame[] =		//窗体资源表(主窗口)
{
	{FRAMEWIN_CreateIndirect,	"KeyBoard",			0,			       0, 0 ,320, 240, FRAMEWIN_CF_MOVEABLE,0},
	{BUTTON_CreateIndirect,	   "6",GUI_ID_TouchCalibrate,  	  10, 115,60, 30},
};

static char _acText[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '='
                        ,0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'
                        ,0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Q'
                        ,0, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ' ' ,' ', ' '
                        ,' ', ' ', ' ', ' ', ' ', 0, 0};

int _ExecKeyboard(void) {
  int i;
  int Key;
  int value;
  BUTTON_Handle ahButton[52];
  BUTTON_Handle hButtonESC;
  EDIT_Handle   hEdit;
  FRAMEWIN_Handle hFrame;
  GUI_RECT rText = {000,0, 320, 20};
  GUI_SetBkColor(GUI_BLUE);  
  GUI_Clear();
  GUI_SetFont(&GUI_Font16B_1);
  GUI_SetColor(GUI_WHITE);
  GUI_DispStringInRect("uc/UI", &rText, GUI_TA_RIGHT | GUI_TA_VCENTER);
  rText.y0 +=20;
  rText.y1 +=20;
  //GUI_DispStringInRect("Touch screen demo", &rText, GUI_TA_RIGHT | GUI_TA_VCENTER);
  //hFrame=FRAMEWIN_CreateAsChild(0,0,320,240,0,"KeyBoard",0,WM_CF_SHOW);
  hFrame=WM_CreateWindow(0,0,320,240,WM_CF_SHOW,0,0);
  /* Create Keyboard Buttons */
  for (i=0; i< 51; i++) {
    int Pos = (i < 47) ? i : i+4;
    int x0 = 5  + 28*(Pos%11);
    int y0 = 100 + 28*(Pos/11);
    char c = _acText[i];
    int Id = c ? c : 1;
    char ac[2] = {0};
    char *s= ac;
    ac[0] = c;
    ahButton[i] = BUTTON_CreateAsChild( x0, y0, 25, 25, hFrame,Id,BUTTON_CF_SHOW );
    BUTTON_SetText   (ahButton[i], s);
    #if GUI_SUPPORT_MEMDEV
      BUTTON_EnableMemdev(ahButton[i]);
    #endif
  }
  ahButton[i] = BUTTON_CreateAsChild( 89, 212, 109, 25, hFrame,' ',BUTTON_CF_SHOW );
  hButtonESC = BUTTON_CreateAsChild( 230, 40, 80, 25, hFrame,GUI_ID_CANCEL,BUTTON_CF_SHOW );
  BUTTON_SetText   (hButtonESC, "ESC");
  hEdit = EDIT_Create( 5, 70, 310, 25, ' ', 80, 0 );
  EDIT_SetFont(hEdit, &GUI_Font8x16);
  BUTTON_SetBkColor(ahButton[49], 0, GUI_RED);
  BUTTON_SetBkColor(ahButton[50], 0, GUI_BLUE);
  /* Handle Keyboard until ESC or ENTER is pressed */
  do {
    Key = WM_WaitKey();
    switch (Key) {
    case GUI_ID_CANCEL:
     break;
    case 0:
    default:
      EDIT_AddKey(hEdit, Key);
    }
  } while ((Key!=GUI_ID_CANCEL) && (Key!=0));
  //
  // Cleanup
  //
  
  value=EDIT_GetValue(hEdit);
  for (i=0; i< GUI_COUNTOF(ahButton); i++) {
    BUTTON_Delete(ahButton[i]);
  }
  BUTTON_Delete(hButtonESC);
  EDIT_Delete(hEdit);
  GUI_EndDialog(hFrame,0);
  return value;
}


static void _cbMainFrame(WM_MESSAGE* pMsg)			//主窗口的回调函数
{
	
	switch (pMsg->MsgId) 
	{
		case WM_NOTIFY_PARENT:
			switch (pMsg->Data.v) 
			{
				case WM_NOTIFICATION_RELEASED:
					if(WM_GetId(pMsg->hWinSrc) == GUI_ID_TouchCalibrate)//KeyBoard
					{
						_ExecKeyboard();
						break;
					}
					if(WM_GetId(pMsg->hWinSrc) == GUI_ID_ButtonExit)	//退出
					{
						GUI_EndDialog(pMsg->hWin, 0);
						break;
					}
					break;
				case WM_NOTIFICATION_SEL_CHANGED:
					break;
				default:
					break;
			}
			break;
		
		default:
			WM_DefaultProc(pMsg);
  }
}


static void _cbBkWindow(WM_MESSAGE* pMsg)			//桌面背景的回调函数
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			GUI_SetBkColor(GUI_RED);
			GUI_Clear();
			GUI_SetFont(&GUI_Font8x16);
			GUI_DispStringHCenterAt("****ucGUI Demo designed by jacky****\n********jackywinter@tom.com*********",320/2,10);
			break;
		default:
			WM_DefaultProc(pMsg);
  }
}


void GUI_Task()
{
	WM_HWIN hFrame;
	GUI_Init();
	GUI_Clear();   //桌面窗口句柄
	WM_SetCallback(WM_HBKWIN,_cbBkWindow);//为窗口窗口设置回调函数
	hFrame = GUI_CreateDialogBox(_aMainFrame,GUI_COUNTOF(_aMainFrame),_cbMainFrame,WM_HBKWIN,0,0);
	GUI_CURSOR_Show();
	for( ; ; )	
	{
		GUI_Exec();
		GUI_Delay(100);
	}
}

void MainTask(void) 
{
	GUI_Task();
}
