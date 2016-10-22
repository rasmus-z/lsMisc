#pragma once
HWND CreateSimpleWindow(HINSTANCE hInst = NULL, 
						LPCTSTR pClassName = NULL, 
						LPCTSTR pWinName = NULL,
						WNDPROC WndProc = NULL,
						void* param=NULL);
