#include "stdafx.h"
#include <windows.h>
#include <tchar.h>

#include "tstring.h"

#include "WritePrivateProfileWString.h"

BOOL WritePrivateProfileWString(const TCHAR *lpszSection, const TCHAR *lpszKey, const TCHAR *lpszIn, const TCHAR *lpszFile)
{
	if(lpszIn==NULL)
	{
		return WritePrivateProfileString(lpszSection, lpszKey, NULL, lpszFile);
	}
	
	char szBuff[4096] = {0};
	int cpylen = min(sizeof(szBuff), (lstrlen(lpszIn)+1)*sizeof(TCHAR));
	memcpy(szBuff, lpszIn, sizeof(szBuff));
	szBuff[ sizeof(szBuff)-1 ] = 0;
	
	return WritePrivateProfileStruct(lpszSection,
		lpszKey,
		(LPVOID)lpszIn,
		sizeof(szBuff),
		lpszFile);
}


BOOL GetPrivateProfileWString(const TCHAR *lpszSection, const TCHAR *lpszKey, const TCHAR *lpszDefault, tstring& tout, const TCHAR *lpszFile)
{
	if(lpszDefault==NULL)
	{
		tout = _T("");
	}
	else
	{
		tout = lpszDefault;
	}

	char szBuff[4096];
	szBuff[0]=0;
	
	if(!GetPrivateProfileStruct(lpszSection,
		lpszKey,
		(LPVOID)szBuff,
		sizeof(szBuff),
		lpszFile))
	{
		DWORD ddd = GetLastError();
		return FALSE;
	}

	tout = (TCHAR*)szBuff;
	return TRUE;
}