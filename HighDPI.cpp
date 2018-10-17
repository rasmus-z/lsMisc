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
#include <windows.h>
#include "HighDPI.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

#ifndef WM_DPICHANGED
#define WM_DPICHANGED                   0x02E0
#endif

namespace Ambiesoft {
	HMODULE ghUser32;

	typedef enum myDPI_AWARENESS {
		myDPI_AWARENESS_INVALID = -1,
		myDPI_AWARENESS_UNAWARE = 0,
		myDPI_AWARENESS_SYSTEM_AWARE = 1,
		myDPI_AWARENESS_PER_MONITOR_AWARE = 2
	} myDPI_AWARENESS;

	typedef void*(WINAPI *fnSetThreadDpiAwarenessContext)(void* dpiContext);
	static fnSetThreadDpiAwarenessContext mySetThreadDpiAwarenessContext;

	typedef void*(WINAPI *fnGetThreadDpiAwarenessContext)(VOID);
	static fnGetThreadDpiAwarenessContext myGetThreadDpiAwarenessContext;

	typedef myDPI_AWARENESS(WINAPI *fnGetAwarenessFromDpiAwarenessContext)(void*);
	fnGetAwarenessFromDpiAwarenessContext myGetAwarenessFromDpiAwarenessContext;

	typedef UINT(WINAPI* fnGetDpiForSystem)(VOID);
	static fnGetDpiForSystem myGetDpiForSystem;

	typedef UINT(WINAPI* fnGetDpiForWindow)(HWND hwnd);
	static fnGetDpiForWindow myGetDpiForWindow;

	static void prepareFunctions()
	{
		mySetThreadDpiAwarenessContext =
			(fnSetThreadDpiAwarenessContext)GetProcAddress(
			ghUser32, "SetThreadDpiAwarenessContext");

		
		myGetThreadDpiAwarenessContext =
			(fnGetThreadDpiAwarenessContext)GetProcAddress(
			ghUser32, "GetThreadDpiAwarenessContext");

		myGetAwarenessFromDpiAwarenessContext =
			(fnGetAwarenessFromDpiAwarenessContext)GetProcAddress(
			ghUser32, "GetAwarenessFromDpiAwarenessContext");

		myGetDpiForSystem =
			(fnGetDpiForSystem)GetProcAddress(
			ghUser32, "GetDpiForSystem");

		myGetDpiForWindow =
			(fnGetDpiForWindow)GetProcAddress(
			ghUser32, "GetDpiForWindow");
	}
	static UINT getWindowsDPI(HWND hWnd)
	{
		UINT uDpi = 96;

		if (!myGetAwarenessFromDpiAwarenessContext || !myGetThreadDpiAwarenessContext)
			return uDpi;

		UINT dpiAwareness = myGetAwarenessFromDpiAwarenessContext(myGetThreadDpiAwarenessContext());
		
		switch (dpiAwareness)
		{
			// Scale the window to the system DPI
		case myDPI_AWARENESS_SYSTEM_AWARE:
			uDpi = myGetDpiForSystem();
			break;

			// Scale the window to the monitor DPI
		case myDPI_AWARENESS_PER_MONITOR_AWARE:
			uDpi = myGetDpiForWindow(hWnd);
			break;
		}
		return uDpi;
	}

	static HHOOK gHook;
	static LRESULT CALLBACK GetMsgProc(
		int    code,
		WPARAM wParam,
		LPARAM lParam
		)
	{
		if (code < 0 || code != HC_ACTION || wParam==PM_NOREMOVE)
		{
			return CallNextHookEx(gHook, code, wParam, lParam);
		}

		MSG* pMsg = reinterpret_cast<MSG*>(lParam);
		if (pMsg->message == WM_DPICHANGED)
		{
			UINT dpi = getWindowsDPI(pMsg->hwnd);
			RECT rcWindow = {};
			GetWindowRect(pMsg->hwnd, &rcWindow);
			LONG width = rcWindow.right - rcWindow.left;
			LONG height = rcWindow.bottom - rcWindow.top;

			rcWindow.right = rcWindow.left + MulDiv(width, dpi, 96);
			rcWindow.bottom = rcWindow.top + MulDiv(height, dpi, 96);
			SetWindowPos(pMsg->hwnd, nullptr, 
				rcWindow.right, rcWindow.top, 
				rcWindow.right - rcWindow.left, 
				rcWindow.bottom - rcWindow.top, 
				SWP_NOZORDER | SWP_NOACTIVATE);
		}

		return CallNextHookEx(gHook, code, wParam, lParam);
	}

	void InitHighDPISupport()
	{
		if (ghUser32)
			return;

		ghUser32 = LoadLibraryA("user32.dll");
		bool ret = false;
		if (!ghUser32)
			return;

		prepareFunctions();
		if (mySetThreadDpiAwarenessContext)
		{
			// DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
			mySetThreadDpiAwarenessContext((void*)-4);
		}
		
		gHook = SetWindowsHookExA(WH_GETMESSAGE, GetMsgProc, (HMODULE)&__ImageBase, GetCurrentThreadId());

	}
}