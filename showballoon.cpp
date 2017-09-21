//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN		// Windows
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <shellapi.h>
#include <Objbase.h>
#include <string>
using namespace std;

#include "showballoon.h"
#include "CreateSimpleWindow.h"

#pragma comment(lib,"Comctl32.lib")

#ifndef ARRAYSIZE
#define ARRAYSIZE(t) sizeof(t)/sizeof(t[0])
#endif

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
	if(pInfoTitle)
		lstrcpynW(nid.szTip, pInfoTitle, ARRAYSIZE(nid.szTip));
	if(pInfoTitle)
		lstrcpynW( nid.szInfoTitle, pInfoTitle, ARRAYSIZE(nid.szInfoTitle));
	if(pInfo)
		lstrcpynW( nid.szInfo, pInfo, ARRAYSIZE(nid.szInfo) );
	
	BOOL ret= Shell_NotifyIconW( dwMessage,&nid);
	//DestroyIcon(sfi.hIcon);
	return ret;
}



LRESULT CALLBACK WaitWindowProc(
   HWND   hwnd,
   UINT   uMsg,
   WPARAM wParam,
   LPARAM lParam
)
{
	static UINT sntimer;
	static int sCounter;
	static int smaxcount;
	switch(uMsg)
	{
	case WM_CREATE:
		sntimer=SetTimer(hwnd, 0, 1000, NULL);
		
		smaxcount = (int)((CREATESTRUCT*)lParam)->lpCreateParams;
		break;
	case WM_TIMER:
		sCounter++;
		if(sCounter>=smaxcount)
		{
			KillTimer(hwnd,sntimer);
			DestroyWindow(hwnd);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}





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
		hWnd = CreateSimpleWindow(
			NULL,
			NULL,
			_T("SimpleWindow"),
			WaitWindowProc,
			(void*)(duration/1000));
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
		// Sleep(duration);
		MSG msg;
		while( GetMessage(&msg, NULL, 0, 0) ) 
		{
			// if( !TranslateAccelerator (msg.hwnd, hAccelTable, &msg) ) 
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		NotifyIconize(hWnd,uTrayID, NIM_DELETE, hIcon, duration,NULL, NULL);
	}
//	DestroyWindow(hBalloon);
//	DestroyIcon(ghIcon);

	if(hwndToDel)
		DestroyWindow(hwndToDel);

	return TRUE;
}

