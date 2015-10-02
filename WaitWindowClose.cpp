#include <windows.h>
#include <tchar.h>
#include <vector>

#define WAITWINDOW_CLASS _T("WaitWindowClass")
struct EWPROCST
{
	std::vector<HWND> vHwnds;
	DWORD dwPID;
};


static BOOL CALLBACK ewproc(HWND hwnd,  LPARAM lParam)
{
	if(!IsWindowVisible(hwnd))
		return TRUE;

	EWPROCST& ewst = *((EWPROCST*)lParam);

	DWORD dwPIDwin = 0;
	GetWindowThreadProcessId(hwnd, &dwPIDwin);
	
	if(ewst.dwPID==dwPIDwin)
	{
		ewst.vHwnds.push_back(hwnd);
	}
	return TRUE;
}


static HWND sHwnd;
static LRESULT CALLBACK WaitWindowProc(
   HWND   hwnd,
   UINT   uMsg,
   WPARAM wParam,
   LPARAM lParam
)
{
	static UINT_PTR sTimer;

	switch(uMsg)
	{
	case WM_CREATE:
		sHwnd = hwnd;
		sTimer = SetTimer(hwnd, 1, 5000, NULL);
		if(sTimer==0)
		{
			DestroyWindow(hwnd);
			break;
		}
		break;

	case WM_TIMER:
		{
			EWPROCST ewst;
			ewst.dwPID=GetCurrentProcessId();
			ewproc(sHwnd, (LPARAM)&ewst);
			if(!EnumWindows(ewproc, (LPARAM)&ewst))
			{
				DestroyWindow(hwnd);
				break;
			}

			if(ewst.vHwnds.size()==0)
			{
				DestroyWindow(hwnd);
				break;
			}
		}
		break;

	case WM_DESTROY:
		KillTimer(hwnd, sTimer);
		break;

	case WM_NCDESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

DWORD WaitWindowClose()
{
	EWPROCST ewst;
	ewst.dwPID = GetCurrentProcessId();

	if(!EnumWindows(ewproc, (LPARAM)&ewst))
	{
		return 0;
	}

	if(ewst.vHwnds.size()==0)
	{
		return 0;
	}


	WNDCLASS wndcls = {0};
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.cbClsExtra = 0;
	wndcls.cbWndExtra = 0;
	wndcls.hInstance = GetModuleHandle(NULL);
	wndcls.hIcon = NULL;
	wndcls.hCursor = LoadCursor(NULL, IDC_IBEAM);
	wndcls.hbrBackground = HBRUSH(COLOR_GRAYTEXT+1);
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = WAITWINDOW_CLASS;
	wndcls.lpfnWndProc = WaitWindowProc;
	if(0==RegisterClass(&wndcls))
		return GetLastError();

	sHwnd = CreateWindowEx(
		0, // exstyle
		WAITWINDOW_CLASS,
		_T(""), // window name
		WS_OVERLAPPEDWINDOW,
		0,0,0,0,
		NULL, // parent
		NULL, // menu
		NULL, // inst
		NULL // lparam
		);
	if(!sHwnd)
		return GetLastError();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}