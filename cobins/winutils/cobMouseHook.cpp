#include "base/cobUtils.h"
#include "cobMouseHook.h"
#include "cobWndBase.h"

NS_COB_BEGIN

#pragma data_seg("mousehook_mydata")
 	 HHOOK glhMouseHook = NULL;    //安装的鼠标勾子句柄 
	 HINSTANCE glhInstance = NULL; //DLL实例句柄 
     HWND  ghHostWnd = NULL;       //host window handle
#pragma data_seg()

//Hook 底层键盘钩子(2009.10.1发现普通键盘钩子在家里台式机的联众四国军旗中失效)
static LRESULT WINAPI MouseProc(int nCode,WPARAM wparam,LPARAM lparam)
{
    if (nCode >= 0)
    {
        LPMOUSEHOOKSTRUCT pMouseHook = (MOUSEHOOKSTRUCT FAR *)lparam;
        if (pMouseHook->hwnd == ghHostWnd || IsChild(ghHostWnd, pMouseHook->hwnd))
            return 0;        
        
        if (wparam == WM_MOUSEMOVE || wparam == WM_NCMOUSEMOVE)
        {
            static DWORD dwLastTickCount = 0;
            if (GetTickCount() - dwLastTickCount < 100)  //延迟100ms
                return 0;
            
            dwLastTickCount = GetTickCount();
        }

        PostMessage(ghHostWnd, WSH_MSG_MOUSE, wparam, (LPARAM)pMouseHook->hwnd);
    }

	return CallNextHookEx(glhMouseHook,nCode,wparam,lparam);
}

BOOL Mouse_StartHook(HWND hHostWnd)
{
   	DWORD ThreadId = 0;

    COBLOG( "Mouse_StartHook: instance = %x\n", glhInstance );

    if (glhMouseHook != NULL)
        return TRUE;

	if (!(glhMouseHook = SetWindowsHookEx(WH_MOUSE, MouseProc, glhInstance, ThreadId)))
	{
		COBLOG( "Mouse_StartHook: err = %d", GetLastError() );
		return FALSE;
	}

    ghHostWnd = hHostWnd;

	return TRUE; 
}

BOOL Mouse_StopHook()
{
	if (glhMouseHook != NULL)
	{
		if (UnhookWindowsHookEx(glhMouseHook))
		{
			glhMouseHook = NULL;
            ghHostWnd = NULL;
            return TRUE;
		}
	}

	return FALSE;
}

NS_COB_END