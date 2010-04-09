#include <windows.h>
#include <tchar.h>

#include "WritePrivateProfileInt.h"

BOOL WritePrivateProfileInt(const TCHAR *lpszSection, const TCHAR *lpszKey, int num, const TCHAR *lpszFile)
{
	TCHAR buf[32];

	wsprintf(buf, _T("%d"), num);
	return WritePrivateProfileString(lpszSection, lpszKey, buf, lpszFile);
}

