#define WIN32_LEAN_AND_MEAN		// Windows
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <Objbase.h>
#include <string>
using namespace std;

#include "showballoon.h"
#include "CreateSimpleWindow.h"

#pragma comment(lib,"Comctl32.lib")

static BOOL NotifyIconize(HWND hWnd, 
						  UINT uID,
						  DWORD dwMessage, 
						  HICON hIcon, 
						  int duration,
						  LPCWSTR pInfoTitle , 
						  LPCWSTR pInfo,
						  DWORD nBalloonIcon = 0)
{
	NOTIFYICONDATAW nid;
	ZeroMemory(&nid,sizeof(nid));
	nid.cbSize = sizeof(NOTIFYICONDATAW);
	nid.hWnd = hWnd;
	nid.uID = uID;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | 0x00000010;
	nid.dwInfoFlags      = nBalloonIcon ;
	nid.uTimeout         = duration;
	nid.uCallbackMessage = 0; //WM_APP_TRAYMESSAGE;
	nid.hIcon = hIcon; //LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MAIN));
	lstrcpynW(nid.szTip, pInfoTitle, ARRAYSIZE(nid.szTip));
	if(pInfoTitle)
		lstrcpynW( nid.szInfoTitle, pInfoTitle, ARRAYSIZE(nid.szInfoTitle));
	if(pInfo)
		lstrcpynW( nid.szInfo, pInfo, ARRAYSIZE(nid.szInfo) );
	
	BOOL ret= Shell_NotifyIconW( dwMessage,&nid);
	//DestroyIcon(sfi.hIcon);
	return ret;
}

	
//static LRESULT CALLBACK WaitWindowProc(
//   HWND   hwnd,
//   UINT   uMsg,
//   WPARAM wParam,
//   LPARAM lParam
//)
//{
//	return DefWindowProc(hwnd,uMsg,wParam,lParam);
//}





BOOL showballoon(HWND hWnd, 
				 const wstring& title,
				 const wstring& text,
				 HICON hIcon, 
				 int duration, 
				 UINT uTrayID, 
				 BOOL bOnlyModify,
				 DWORD dwBalloonIcon)
{
	// CoInitialize(NULL);

	HWND hwndToDel = NULL;
	if(hWnd==NULL)
	{
		hWnd = CreateSimpleWindow();
		hwndToDel= hWnd;
	}

	//InitCommonControls();
	//HWND hBalloon = CreateWindowW(L"tooltips_class32",
	//	NULL,
	//	WS_POPUP|0x02|0x40|0x01,
	//	CW_USEDEFAULT,
	//	CW_USEDEFAULT,
	//	CW_USEDEFAULT,
	//	CW_USEDEFAULT,
	//	NULL,
	//	NULL,
	//	NULL,
	//	NULL);
	//if(!hBalloon)
	//{
	//	// MessageBoxW(NULL, L"balloon",NULL,MB_ICONERROR);
	//	return FALSE;
	//}
	//if(!SendMessageW(hBalloon,
	//	TTM_SETTITLE, // Adds a standard icon and title string to a ToolTip    
	//	1,
	//	(LPARAM)L"title"))
	//{
	//	return FALSE;
	//}


	if(!bOnlyModify)
	{
		NotifyIconize(hWnd,uTrayID,NIM_DELETE, hIcon, duration, NULL, NULL);
		if(!NotifyIconize(hWnd,uTrayID,NIM_ADD, hIcon, duration,NULL, NULL))
		{
			// MessageBoxA(NULL, "NotifyAdd",NULL,MB_ICONERROR);
			return FALSE;
		}
	}


	if(!NotifyIconize(hWnd,uTrayID, NIM_MODIFY, hIcon, duration,title.c_str(), text.c_str(),dwBalloonIcon  ))
	{
		// MessageBoxA(NULL, "NotifyModify",NULL,MB_ICONERROR);
		return FALSE;
	}

	if(!bOnlyModify)
	{
		Sleep(duration);
		NotifyIconize(hWnd,uTrayID, NIM_DELETE, hIcon, duration,NULL, NULL);
	}
//	DestroyWindow(hBalloon);
//	DestroyIcon(ghIcon);

	if(hwndToDel)
		DestroyWindow(hwndToDel);

	return TRUE;
}

