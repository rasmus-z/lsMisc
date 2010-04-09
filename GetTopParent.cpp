#include <windows.h>
#include "GetTopParent.h"

HWND GetTopParent(HWND h)
{
	HWND top = h;
	HWND pre ;
	do {
		pre = top;
	} while( (top = GetParent(pre))!=NULL );
	return pre;
}
