#include "cobKbHook.h"
#include "base/cobUtils.h"
#include "cobWndBase.h"

NS_COB_BEGIN

static HINSTANCE glhInstance=NULL;   //DLL实例句柄 
static HHOOK glhKeyboardHook = NULL; //安装的鼠标勾子句柄
static HWND  ghHostWnd = NULL;       //host window handle

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	COBLOG("kbhook: DllMain glhInstance = %x", hInstance);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		glhInstance = hInstance;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{

	}

	return 1;
}

//Hook 底层键盘钩子(2009.10.1发现普通键盘钩子在家里台式机的联众四国军旗中失效)
static LRESULT WINAPI KeyboardProc(int nCode,WPARAM wparam,LPARAM lparam)
{
	PKBDLLHOOKSTRUCT pKeyInfo  = (PKBDLLHOOKSTRUCT)lparam;
    if (nCode >= 0)
    {
        LPARAM lParam = 1;
        lParam += pKeyInfo->scanCode << 16;
        lParam += pKeyInfo->flags << 24;
        if (pKeyInfo->flags == 0x80)
        {
            lParam += 1 << 30; //Specifies the previous key state. The value is always 1 for a WM_KEYUP message. (MSDN)
        }

        //COBLOG("KeyHook: curprocess=%x\n", GetCurrentProcessId());

        if (ghHostWnd != NULL)
        {
            PostMessage(ghHostWnd, WSH_MSG_KEY, (WPARAM)pKeyInfo->vkCode, lParam);
        }

        //COBLOG("scancode=%x,flags=%x,time=%x, dwMsg=%x\n", pKeyInfo->scanCode, pKeyInfo->flags, pKeyInfo->time, lParam);
    }

	return CallNextHookEx(glhKeyboardHook, nCode, wparam, lparam);
}

BOOL Kb_StartHook(HWND hHostWnd)
{
   	DWORD ThreadId = 0;
    
    if (glhKeyboardHook != NULL)
    {
        COBLOG( "KB_StartHook: already hooked\n");
        return FALSE;
    }
    
    if (NULL == (glhKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, glhInstance, ThreadId)))
    {
        COBLOG( "KB_StartHook: Hook Keyboard error = %d", GetLastError() );
        return FALSE;
    }
    
    ghHostWnd = hHostWnd;
    
    COBLOG("glhKeyboardHook=%x\n", glhKeyboardHook);
    
    return TRUE; 
}

BOOL Kb_StopHook(HWND hHostWnd)
{
    if(glhKeyboardHook && UnhookWindowsHookEx(glhKeyboardHook))
    {
        glhKeyboardHook = NULL;
        return TRUE;
    }
    else
    {
        COBLOG("stop hook failed\n");
    }
    return FALSE;
}

NS_COB_END