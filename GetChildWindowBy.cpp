#include "stdafx.h"

#include <Windows.h>

#include "GetChildWindowBy.h"


struct ContextData {
	LPCWSTR pName;
	HWND hWndResultChild;
};

static BOOL CALLBACK enumProc(
	_In_ HWND   hwnd,
	_In_ LPARAM lParam
	)
{
	ContextData* pCxt = (ContextData*)lParam;
	
	TCHAR szT[1024]; szT[0] = 0;
	GetWindowText(hwnd, szT, 1024);
	if (lstrcmp(szT, pCxt->pName) == 0)
	{
		pCxt->hWndResultChild = hwnd;

		// no continue;
		return FALSE;
	}
	// continue
	return TRUE;
}

HWND GetChildWindowByName(HWND hwndParent, LPCWSTR pName)
{
	ContextData context = {};
	context.pName = pName;
	
	EnumChildWindows(hwndParent, enumProc, (LPARAM)&context);
	return context.hWndResultChild;
}