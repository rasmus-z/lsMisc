#include <tchar.h>
#include <windows.h>
#include "IsRegKeyExists.h"

BOOL IsRegKeyExists(HKEY hRoot, LPCTSTR pKey)
{
	HKEY h = NULL;
	BOOL bRet = ( ERROR_SUCCESS == RegOpenKeyEx( hRoot, 
		pKey,
		0,
		KEY_QUERY_VALUE,
		&h) );

	if ( h )
		RegCloseKey(h);

	return bRet;
}
