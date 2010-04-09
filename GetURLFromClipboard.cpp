#include <windows.h>
#include <tchar.h>

#include "GetURLFromClipboard.h"
#include "RemoveLastInValidURLChar.h"

BOOL GetURLFromClipboard(HWND hWnd,LPTSTR pURL, size_t dwSize)
{
	if( pURL==NULL )
		return FALSE;

	if( !IsClipboardFormatAvailable(CF_TEXT) )
		return FALSE;

	if( !OpenClipboard(hWnd) )
		return FALSE;

	LPCTSTR pData = (LPCTSTR)GetClipboardData(CF_TEXT);
	size_t nDataLen = _tcslen(pData);
	if( nDataLen >= dwSize ) {
		CloseClipboard();
		return FALSE;
	}

	LPTSTR pBuf = _tcsdup(pData);
	CloseClipboard();

	pURL[0] = 0;

	if(pBuf==NULL)
		return FALSE;

	LPTSTR pEnd = _tcsstr(pBuf, _T("\n"));
	if(pEnd) {
		*pEnd = 0;
		pEnd--;
		if(*pEnd==_T('\r') && pBuf <= pEnd) {
			*pEnd = 0;
		}
	}

	LPTSTR pHttpStart = _tcsstr(pBuf, _T("http://"));
	if(pHttpStart!=NULL) 
	{
		RemoveLastInValidURLChar(pHttpStart);
		_tcscpy(pURL, pHttpStart);
	} 
	else 
	{
		LPTSTR pFtpStart = _tcsstr(pBuf, _T("ftp://"));
		if(pFtpStart!=NULL) 
		{
			RemoveLastInValidURLChar(pFtpStart);
			_tcscpy(pURL, pFtpStart);
		} 
		else if( !( (_tcslen(pBuf)+sizeof(_T("http://"))) >= dwSize) ) 
		{
			RemoveLastInValidURLChar(pBuf);
			_tcscpy(pURL, _T("http://"));
			_tcscat(pURL, pBuf);
		}
	}
	free(pBuf);
	return TRUE;
}
