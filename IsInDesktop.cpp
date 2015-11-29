#include "stdafx.h"
#include <windows.h>
#include "IsInDesktop.h"
BOOL IsInDesktop(HWND h, int x, int y)
{
	if ( !IsWindow(h) )
		return FALSE;

	RECT r;
	if (!GetWindowRect(h, &r))
		return FALSE;

	RECT rDesk;
	if ( !GetWindowRect(GetDesktopWindow(), &rDesk) )
		return FALSE;

	return (rDesk.left < x) &&
		   (x < rDesk.right) &&
		   (rDesk.left < (x+(r.right-r.left))) && 
		   ((x+(r.right-r.left)) < rDesk.right) &&
		   (rDesk.top < y ) && (y < rDesk.bottom) &&
		   (rDesk.top < (y+(r.bottom-r.top))) &&  
		   ((y+(r.bottom-r.top)) < rDesk.bottom);
}

