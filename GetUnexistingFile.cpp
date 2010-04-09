#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "tstring.h"
#include "GetUnexistingFile.h"

tstring GetUnexistingFile(LPCTSTR pDir, LPCTSTR pPrefix, LPCTSTR pSuffix)
{
	tstring ret;
	
	TCHAR szFileName[MAX_PATH] = {0};
	if ( pDir )
	{
		lstrcpy(szFileName, pDir);
		PathAddBackslash(szFileName);
	}


	for ( int i=1;;++i )
	{
		TCHAR szTry[MAX_PATH] = {0};
		wsprintf(szTry, _T("%d"), i);

		tstring strT(szFileName);
		if ( pPrefix )
			strT += pPrefix;

		strT += szTry;

		if ( pSuffix )
			strT += pSuffix;

		if ( GetFileAttributes(strT.c_str()) == 0xFFFFFFFF )
		{
			ret = strT;
			break;
		}
	}


	return ret;
}