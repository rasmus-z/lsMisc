#include <windows.h>
#include <tchar.h>
#include "CreateSimpleWindow.h"

HWND CreateSimpleWindow(HINSTANCE hInst, 
						LPCTSTR pClassName, 
						LPCTSTR pWinName,
						WNDPROC WndProc)
{
	hInst = hInst ? hInst : GetModuleHandle(NULL);
	pClassName = pClassName ? pClassName : TEXT("CreateSimpleWindowClass");
	WndProc = WndProc ? WndProc : DefWindowProc;

	WNDCLASSEX wcex;
	if(!GetClassInfoEx(hInst, pClassName, &wcex))
	{
		wcex.cbSize = sizeof(WNDCLASSEX); 

		wcex.style			= 0; //CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInst;
		wcex.hIcon			= NULL;
		wcex.hCursor		= NULL;
		wcex.hbrBackground	= NULL; //(HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= pClassName;
		wcex.hIconSm		= NULL;

		if(!RegisterClassEx( &wcex ))
			return NULL;
	}

	return CreateWindowEx(0, 
		pClassName,
		pWinName,
		0,
		0,
		0,
		0,
		0,
		NULL,
		NULL,
		hInst,
		NULL);
}