#define WIN32_LEAN_AND_MEAN		// Windows
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <Objbase.h>
#include <string>
using namespace std;

#include "showballoon.h"

#pragma comment(lib,"Comctl32.lib")

static BOOL NotifyIconize(HWND hWnd, UINT uID, DWORD dwMessage, HICON hIcon, LPCWSTR pInfoTitle , LPCWSTR pInfo)
{
	NOTIFYICONDATAW nid;
	ZeroMemory(&nid,sizeof(nid));
	nid.cbSize = sizeof(NOTIFYICONDATAW);
	nid.hWnd = hWnd;
	nid.uID = uID;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | 0x00000010;
	nid.dwInfoFlags      = 0x00000001;
	nid.uTimeout         = 300;
	nid.uCallbackMessage = 0; //WM_APP_TRAYMESSAGE;
	nid.hIcon = hIcon; //LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MAIN));
	lstrcpyW(nid.szTip, L"dater");
	if(pInfoTitle)
		lstrcpyW( nid.szInfoTitle, pInfoTitle );
	if(pInfo)
		lstrcpyW( nid.szInfo, pInfo );
	
	BOOL ret= Shell_NotifyIconW( dwMessage,&nid);
	//DestroyIcon(sfi.hIcon);
	return ret;
}

	



BOOL showballoon(HWND hWnd, const wstring& title, const wstring& text, HICON hIcon, UINT uTrayID, BOOL bOnlyModify)
{
	CoInitialize(NULL);

/**
	if(!iconexe.empty())
	{
		SHFILEINFOW sfi={0};
		SHGetFileInfoW(iconexe.c_str(),
					   0,
					   &sfi, 
					   sizeof(SHFILEINFO), 
					   // SHGFI_SYSICONINDEX|
					   SHGFI_ICON|
					   SHGFI_SMALLICON);


		ghIcon = sfi.hIcon;
	} 	
**/	

	InitCommonControls();
	HWND hBalloon = CreateWindowW(L"tooltips_class32",
		NULL,
		WS_POPUP|0x02|0x40|0x01,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		NULL,
		NULL);
	if(!hBalloon)
	{
		// MessageBoxW(NULL, L"balloon",NULL,MB_ICONERROR);
		return FALSE;
	}
	if(!SendMessageW(hBalloon,
		TTM_SETTITLE, // Adds a standard icon and title string to a ToolTip    
		1,
		(LPARAM)L"title"))
	{
		// MessageBoxW(NULL, L"balloon sendmessage",NULL,MB_ICONERROR);
		return FALSE;
	}

	if(!bOnlyModify)
	{
		NotifyIconize(hWnd,uTrayID,NIM_DELETE, hIcon, NULL, NULL);
		if(!NotifyIconize(hWnd,uTrayID,NIM_ADD, hIcon, NULL, NULL))
		{
			// MessageBoxA(NULL, "NotifyAdd",NULL,MB_ICONERROR);
			return FALSE;
		}
	}


	if(!NotifyIconize(hWnd,uTrayID, NIM_MODIFY, hIcon, title.c_str(), text.c_str() ))
	{
		// MessageBoxA(NULL, "NotifyModify",NULL,MB_ICONERROR);
		return FALSE;
	}

	if(!bOnlyModify)
	{
		Sleep(5000);
		NotifyIconize(hWnd,uTrayID, NIM_DELETE, hIcon, NULL, NULL);
	}
	DestroyWindow(hBalloon);
//	DestroyIcon(ghIcon);
	return TRUE;
}

