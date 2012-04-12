#include <windows.h>
#include "CreateSimpleWindow.h"

HWND CreateSimpleWindow(HINSTANCE hInst, 
						LPCTSTR pClassName, 
						LPCTSTR pWinName,
						WNDPROC WndProc)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style		= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= GetModuleHandle(NULL);
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= pClassName;
	wcex.hIconSm		= NULL;

	if(!RegisterClassEx( &wcex ))
		return NULL;

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