#include "stdafx.h"
#include <Windows.h>
#include "HandleUtility.h"

#ifdef __cplusplus_cli
void ClearHandle(HANDLE% h)
#else
void ClearHandle(HANDLE& h)
#endif
{
	if (h)
	{
		CloseHandle(h);
		h = NULL;
	}
}

BOOL IsProcessActive(HANDLE& h)
{
	return WAIT_TIMEOUT == WaitForSingleObject(h,0);
}