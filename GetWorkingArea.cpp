#include <windows.h>

BOOL GetWorkingArea(HWND h,RECT* pRetRect)
{
	if(!h)
		return FALSE;

	RECT r;
	if(!GetWindowRect(h, &r))
		return FALSE;

	HMONITOR hm = MonitorFromRect(&r,MONITOR_DEFAULTTONEAREST);
	if(!hm)
		return FALSE;

	MONITORINFO mi;
	mi.cbSize=sizeof(mi);
	if(!GetMonitorInfo(hm, &mi))
		return FALSE;

	*pRetRect = mi.rcWork;
	return TRUE;
}