#include <windows.h>
#include <tchar.h>
#include <Shlobj.h>
#include <assert.h>
#include <shlwapi.h>

#include "GetDesktopDir.h"

BOOL GetDesktopDir(LPTSTR lpBuf, DWORD dwSize)
{
	if(lpBuf==NULL || dwSize < MAX_PATH)
		return FALSE;

	LPITEMIDLIST lpDeskTop = NULL;
	if( SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP,&lpDeskTop)) )
	{
		assert(lpDeskTop);

		if( !SHGetPathFromIDList(lpDeskTop, lpBuf) )
			lpBuf[0] = _T('\0');
		else
			PathAddBackslash(lpBuf);
	}
	LPMALLOC pMalloc = NULL;
	if( SUCCEEDED(SHGetMalloc(&pMalloc)) )
	{
		pMalloc->Free(lpDeskTop);
		pMalloc->Release();
	}

	if( lpBuf[0] == _T('\0') )
	{
		assert(0);
		lstrcpy( lpBuf, _T("") );
	}

	return TRUE;
}

