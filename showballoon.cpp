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
#include <shlwapi.h>
#include <Objbase.h>
#include <string>
using namespace std;

#include "showballoon.h"
#include "CreateSimpleWindow.h"

#pragma comment(lib,"Comctl32.lib")
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"shell32.lib")

#ifndef ARRAYSIZE
#define ARRAYSIZE(t) sizeof(t)/sizeof(t[0])
#endif

namespace Ambiesoft {

enum {
	WM_APP_TRAYMESSAGE = WM_APP+1,
};

// https://www.codeproject.com/Articles/4768/Basic-use-of-Shell-NotifyIcon-in-Win
typedef HRESULT (CALLBACK* DLLGETVERSIONPROC)(DLLVERSIONINFO *);
static ULONGLONG GetDllVersion(LPCTSTR lpszDllName)
{
    ULONGLONG ullVersion = 0;
	HINSTANCE hinstDll;
    hinstDll = LoadLibrary(lpszDllName);
    if(hinstDll)
    {
        DLLGETVERSIONPROC pDllGetVersion;
        pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");
        if(pDllGetVersion)
        {
            DLLVERSIONINFO dvi;
            HRESULT hr;
            ZeroMemory(&dvi, sizeof(dvi));
            dvi.cbSize = sizeof(dvi);
            hr = (*pDllGetVersion)(&dvi);
            if(SUCCEEDED(hr))
				ullVersion = MAKEDLLVERULL(dvi.dwMajorVersion, dvi.dwMinorVersion,0,0);
        }
        FreeLibrary(hinstDll);
    }
    return ullVersion;
}

#if _WIN32_WINNT==0x5000
#pragma error("Do not use _WIN32_WINNT==0x5000, it has a bug in header.")
#endif

static BOOL NotifyIconizeW(HWND hWnd, 
						  UINT uID,
						  DWORD dwMessage, 
						  HICON hIcon, 
						  int duration,
						  LPCWSTR pInfoTitle, 
						  LPCWSTR pInfo,
						  DWORD nBalloonIcon = 0)
{
//	InitCommonControls();
	//INITCOMMONCONTROLSEX iccs={0};
	//iccs.dwSize = sizeof(iccs);
	//iccs.dwICC = 0xFFFFFFFF;
	//InitCommonControlsEx(&iccs);

	NOTIFYICONDATAW nid;
	ZeroMemory(&nid,sizeof(nid));

	//ULONGLONG ullVersion = GetDllVersion(_T("Shell32.dll"));
	//if(ullVersion >= MAKEDLLVERULL(5, 0,0,0))
	//	nid.cbSize = sizeof(NOTIFYICONDATAW);
	//else 
	//	nid.cbSize = NOTIFYICONDATAW_V2_SIZE;

	nid.cbSize = sizeof(NOTIFYICONDATAW);
	nid.hWnd = hWnd;
	nid.uID = uID;
	nid.uFlags = NIF_ICON;// | NIF_TIP | NIF_INFO | NIF_MESSAGE;
	nid.dwInfoFlags = nBalloonIcon;
	if(dwMessage != NIM_DELETE)
		nid.uTimeout = duration;
	
	nid.uCallbackMessage = WM_APP_TRAYMESSAGE;
	nid.hIcon = hIcon;
	//if(pInfoTitle)
	//{
	//	nid.uFlags |= NIF_TIP;
	//	lstrcpynW(nid.szTip, pInfoTitle, ARRAYSIZE(nid.szTip));
	//}
	if(dwMessage==NIM_ADD || dwMessage==NIM_MODIFY)
	{
		if(pInfoTitle)
		{
			nid.uFlags|=NIF_INFO;
			lstrcpynW(nid.szInfoTitle, pInfoTitle, ARRAYSIZE(nid.szInfoTitle));
		}
		if(pInfo)
		{
			nid.uFlags|=NIF_INFO;
			lstrcpynW(nid.szInfo, pInfo, ARRAYSIZE(nid.szInfo) );
		}
	}
	BOOL ret= Shell_NotifyIconW(dwMessage,&nid);
	
	return ret;
}
static BOOL NotifyIconizeA(HWND hWnd, 
						  UINT uID,
						  DWORD dwMessage, 
						  HICON hIcon, 
						  int duration,
						  LPCSTR pInfoTitle, 
						  LPCSTR pInfo,
						  DWORD nBalloonIcon = 0)
{
//	InitCommonControls();
	//INITCOMMONCONTROLSEX iccs={0};
	//iccs.dwSize = sizeof(iccs);
	//iccs.dwICC = 0xFFFFFFFF;
	//InitCommonControlsEx(&iccs);

	NOTIFYICONDATAA nid;
	ZeroMemory(&nid,sizeof(nid));

	//ULONGLONG ullVersion = GetDllVersion(_T("Shell32.dll"));
	//if(ullVersion >= MAKEDLLVERULL(5, 0,0,0))
	//	nid.cbSize = sizeof(NOTIFYICONDATAW);
	//else 
	//	nid.cbSize = NOTIFYICONDATAW_V2_SIZE;

	nid.cbSize = 952; //sizeof(NOTIFYICONDATAA_V2_SIZE);
	nid.hWnd = hWnd;
	nid.uID = uID;
	
	nid.dwInfoFlags = nBalloonIcon;
	if(dwMessage != NIM_DELETE)
		nid.uTimeout = duration;
	
	nid.uFlags |= NIF_MESSAGE;
	nid.uCallbackMessage = WM_APP_TRAYMESSAGE;

	if(hIcon)
	{
		nid.uFlags |= NIF_ICON;
		nid.hIcon = hIcon;
	}
	//if(pInfoTitle)
	//{
	//	nid.uFlags |= NIF_TIP;
	//	lstrcpynW(nid.szTip, pInfoTitle, ARRAYSIZE(nid.szTip));
	//}
	if(dwMessage==NIM_ADD || dwMessage==NIM_MODIFY)
	{
		if(pInfoTitle)
		{
			nid.uFlags|=NIF_INFO;
			lstrcpynA(nid.szInfoTitle, pInfoTitle, ARRAYSIZE(nid.szInfoTitle));
		}
		if(pInfo)
		{
			nid.uFlags|=NIF_INFO;
			lstrcpynA(nid.szInfo, pInfo, ARRAYSIZE(nid.szInfo) );
		}
	}
	BOOL ret= Shell_NotifyIconA(dwMessage,&nid);
	
	return ret;
}


struct WaitParam
{
	int nNim;
	int uTrayID;
	HICON hIcon;
	int duration;
	wstring title;
	wstring text;
	int dwBalloonIcon;
	
	int smaxcount;
	WaitParam(
		int nNim,
		int uTrayID,
		HICON hIcon,
		int duration,
		wstring title,
		wstring text,
		DWORD dwBalloonIcon)
	{
		this->nNim=nNim;
		this->uTrayID=uTrayID;
		this->hIcon=hIcon;
		this->duration=duration;
		this->title=title;
		this->text=text;
		this->dwBalloonIcon=dwBalloonIcon;
	}

	void operator()(HWND hWnd)
	{
		NotifyIconizeW(hWnd,
					uTrayID, 
					NIM_MODIFY,
					NULL, 
					duration,title.c_str(), 
					text.c_str(),
					dwBalloonIcon);
	}
};

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
	static WaitParam* spWaitParam;
	switch(uMsg)
	{
	case WM_CREATE:
		sntimer=SetTimer(hwnd, 0, 1000, NULL);
		
		spWaitParam = (WaitParam*)((CREATESTRUCT*)lParam)->lpCreateParams;
		smaxcount = spWaitParam->smaxcount;

		break;
	case WM_TIMER:
		sCounter++;
		if(sCounter>=smaxcount)
		{
			KillTimer(hwnd,sntimer);
			DestroyWindow(hwnd);
		}
		//else
		//{
		//	if(spWaitParam && ((smaxcount-sCounter) > 5))
		//	{
		//		(*spWaitParam)(hwnd);
		//	}
		//}
		break;
	case WM_APP_TRAYMESSAGE:
		// MessageBoxA(NULL,"ddd","",0);
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
	WaitParam param(
		NIM_MODIFY,
		uTrayID,
		hIcon,
		duration,
		title,
		text,
		dwBalloonIcon);


	if(hWnd==NULL)
	{
		param.smaxcount = (duration/1000);
		hWnd = CreateSimpleWindow(
			NULL,
			NULL,
			_T("SimpleWindow"),
			WaitWindowProc,
			0,
			0,
			(void*)&param);
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
		NotifyIconizeW(hWnd,uTrayID,NIM_DELETE, hIcon, duration, NULL, NULL);
		if(!NotifyIconizeW(hWnd,uTrayID,NIM_ADD, hIcon, duration,NULL, NULL))
		{
			return FALSE;
		}
	}



	if(!NotifyIconizeW(hWnd,uTrayID, NIM_MODIFY, hIcon, duration,title.c_str(), text.c_str(),dwBalloonIcon  ))
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
		NotifyIconizeW(hWnd,uTrayID, NIM_DELETE, hIcon, duration,NULL, NULL);
	}
//	DestroyWindow(hBalloon);
//	DestroyIcon(ghIcon);

	if(hwndToDel)
		DestroyWindow(hwndToDel);

	return TRUE;
}

}