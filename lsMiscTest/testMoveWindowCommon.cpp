#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <iostream>

#include "../CenterWindow.h"
#include "../CreateSimpleWindow.h"

using namespace std;

void testMoveWindowCommon()
{
	HWND h = CreateSimpleWindow();
	ShowWindow(h, SW_SHOW);
	MoveWindow(h,0,0,300,300,TRUE);
	MoveWindowCommon(h, MOVEWINDOW_BOTTOMRIGHT);
	// WaitSimpleWindowClose(h);
}