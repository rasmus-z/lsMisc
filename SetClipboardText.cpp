#include <windows.h>
// #include "tstring.h"
// #include <string>
#include "SetClipboardText.h"

BOOL SetClipboardTextA(HWND hWnd, LPCSTR pSTR)
{
	if ( !pSTR || pSTR[0]==0 )
	{
		return FALSE;
	}

	size_t strsize = lstrlen(pSTR);
	HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,
		strsize+sizeof(TCHAR));
	
	if(h==NULL)
		return FALSE;

	BOOL bRet = FALSE;
	LPTSTR p = (LPTSTR)GlobalLock(h);
	lstrcpy(p, pSTR);
	if( OpenClipboard(hWnd) )
	{
		if( EmptyClipboard() )
		{
			SetClipboardData(CF_TEXT,h);
		}
		CloseClipboard();
		GlobalUnlock(h);
		bRet = TRUE;
	}
	else
	{
		GlobalFree(h);
		bRet = FALSE;
	}

	return bRet;
}

BOOL SetClipboardTextW(HWND hWnd, LPCWSTR pSTR)
{
	if ( !pSTR || pSTR[0]==0 )
	{
		return FALSE;
	}

	size_t strsize = lstrlenW(pSTR) * sizeof(WCHAR);
	HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,
		strsize+sizeof(WCHAR));
	
	if(h==NULL)
		return FALSE;

	BOOL bRet = FALSE;
	LPWSTR p = (LPWSTR)GlobalLock(h);
	lstrcpyW(p, pSTR);
	if( OpenClipboard(hWnd) )
	{
		if( EmptyClipboard() )
		{
			SetClipboardData(CF_UNICODETEXT,h);
		}
		CloseClipboard();
		GlobalUnlock(h);
		bRet = TRUE;
	}
	else
	{
		GlobalFree(h);
		bRet = FALSE;
	}

	return bRet;
}

