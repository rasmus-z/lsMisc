#include <windows.h>
#include <comdef.h>
#include "AnyCloser.h"
#include "Registory.h"

BOOL TrxIsRegKeyExists(HKEY hRoot, LPCTSTR pSub)
{
	BOOL bRet = FALSE;
	HKEY hRet = NULL;
	if (ERROR_SUCCESS==RegOpenKeyEx(hRoot, pSub, 0, KEY_QUERY_VALUE, &hRet))
	{
		bRet = TRUE;
		RegCloseKey(hRet);
	}

	return bRet;
}

BOOL TrxRegGetValue(HKEY hRoot, LPCTSTR pSub, LPCTSTR pName, tstring& value)
{
	HKEY hKey;

	if ( ERROR_SUCCESS  != RegOpenKeyEx( hRoot,
				pSub,
				0,
				KEY_QUERY_VALUE ,
				&hKey))
	{
		return FALSE;
	}

	RegCloser closer(hKey);

	DWORD dwType = REG_SZ;
	BYTE* szT[512] = {0};
	DWORD dwSize = sizeof(szT)-1;
	if ( ERROR_SUCCESS != RegQueryValueEx(
			hKey,
			pName,
			0,
			&dwType,
			(BYTE*)szT,
			&dwSize ) )
	{
		return FALSE;
	}

	if ( dwType == REG_SZ )
	{
		value = (LPCTSTR)szT;
	}
	else if ( dwType == REG_DWORD )
	{
		DWORD d = *((DWORD*)szT);
		if ( d == 0 )
			value = "0";
		else if ( d == 1 )
			value = "1";
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}



