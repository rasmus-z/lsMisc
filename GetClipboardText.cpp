#include <windows.h>
#include "tstring.h"
#include "GetClipboardText.h"

using namespace std;
BOOL GetClipboardTextA(HWND hWnd, string& strData)
{
	if( !IsClipboardFormatAvailable(CF_TEXT) )
		return FALSE;
	if( !OpenClipboard(hWnd) )
		return FALSE;

	struct ClipboardCloser {
		~ClipboardCloser() {::CloseClipboard();}
	} closer;

	HANDLE hData = ::GetClipboardData(CF_TEXT);
	if( NULL == hData )
		return FALSE;

	LPCSTR p = (LPCSTR)GlobalLock(hData);
	if(!p)
		return FALSE;

	strData = p;
	GlobalUnlock(hData);
		
		
	return TRUE;
}

BOOL GetClipboardTextW(HWND hWnd, wstring& strData)
{
	if( !IsClipboardFormatAvailable(CF_UNICODETEXT) )
		return FALSE;
	if( !OpenClipboard(hWnd) )
		return FALSE;

	struct ClipboardCloser {
		~ClipboardCloser() {::CloseClipboard();}
	} closer;

	HANDLE hData = ::GetClipboardData(CF_UNICODETEXT);
	if( NULL == hData )
		return FALSE;

	LPCWSTR p = (LPCWSTR)GlobalLock(hData);
	if(!p)
		return FALSE;

	strData = p;
	GlobalUnlock(hData);
		
		
	return TRUE;
}
