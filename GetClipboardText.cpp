#include "stdafx.h"
#include <windows.h>
#include "tstring.h"

#include "UTF16toUTF8.h"

#include "GetClipboardText.h"

using namespace std;
static BOOL getClipboardTextDataA(HWND hWnd, int cf, string& strData)
{
	if( !IsClipboardFormatAvailable(cf) )
		return FALSE;
	if( !OpenClipboard(hWnd) )
		return FALSE;

	struct ClipboardCloser {
		~ClipboardCloser() {::CloseClipboard();}
	} closer;

	HANDLE hData = ::GetClipboardData(cf);
	if( NULL == hData )
		return FALSE;

	LPCSTR p = (LPCSTR)GlobalLock(hData);
	if(!p)
		return FALSE;

	strData = p;
	GlobalUnlock(hData);
		
		
	return TRUE;
}
BOOL GetClipboardTextA(HWND hWnd, string& strData)
{
	return getClipboardTextDataA(hWnd, CF_TEXT, strData);
}

static BOOL getHtmlPosition(LPCSTR pStr, LPCSTR pKey, int& val)
{
	if(!pStr || pStr[0]==0)
		return FALSE;

	if(!pKey || pKey[0]==0)
		return FALSE;

	LPCSTR pS = strstr(pStr, pKey);
	if(!pS)
		return FALSE;

	pS += strlen(pKey);
	BOOL zeroFilled=false;
	string result;
	for(; *pS!=0 && ('0' <= *pS && *pS <= '9') ; ++pS)
	{
		if(!zeroFilled)
		{
			if('0'==*pS)
				continue;
			zeroFilled = true;
			result += *pS;
		}
		else
		{
			result += *pS;
		}
	}

	val = atoi(result.c_str());
	return TRUE;
}

BOOL GetClipboardHtmlA(HWND hWnd, string& strData)
{
	static int cf = RegisterClipboardFormat(L"HTML Format");

	string strT;
	if(!getClipboardTextDataA(hWnd, cf, strT))
		return FALSE;

	int start = -1;
	if(!getHtmlPosition(strT.c_str(), "StartHTML:", start))
		return FALSE;
	if(start < 0)
		return FALSE;

	int end=-1;
	if(!getHtmlPosition(strT.c_str(), "EndHTML:", end))
		return FALSE;
	if(end < 0)
		return FALSE;

	if(start >= end)
		return FALSE;

	strData = strT.substr(start, end-start);
	return TRUE;
}









static BOOL getClipboardTextDataW(HWND hWnd, int cf, wstring& strData)
{
	if( !IsClipboardFormatAvailable(cf) )
		return FALSE;
	if( !OpenClipboard(hWnd) )
		return FALSE;

	struct ClipboardCloser {
		~ClipboardCloser() {::CloseClipboard();}
	} closer;

	HANDLE hData = ::GetClipboardData(cf);
	if( NULL == hData )
		return FALSE;

	LPCWSTR p = (LPCWSTR)GlobalLock(hData);
	if(!p)
		return FALSE;

	strData = p;
	GlobalUnlock(hData);
		
		
	return TRUE;
}
BOOL GetClipboardTextW(HWND hWnd, wstring& strData)
{
	return getClipboardTextDataW(hWnd, CF_UNICODETEXT, strData);
}
BOOL GetClipboardHtmlW(HWND hWnd, wstring& strData)
{
	string strU8;
	if(!GetClipboardHtmlA(hWnd, strU8))
		return FALSE;

	if(!UTF8toUTF16((const LPBYTE)strU8.c_str(), strData))
		return FALSE;

	return TRUE;
}
