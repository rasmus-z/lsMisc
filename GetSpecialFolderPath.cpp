#include "StdAfx.h"

#ifndef _WINDOWS_
#pragma message("--including windows.h")
#include <windows.h>
#endif

#ifndef _INC_TCHAR
#pragma message("--including tchar.h")
#include <tchar.h>
#endif


#ifndef _SHLOBJ_H_
#pragma comment("including shlobj.h")
#include <Shlobj.h>
#endif

#include "tstring.h"
#include "GetSpecialFolderPath.h"


// CSIDL_LOCAL_APPDATA
// CSIDL_APPDATA


BOOL GetSpecialFolderPath( HWND hWnd, int nFolder, TCHAR *Path )
{
	IMalloc    *pMalloc;
	ITEMIDLIST *pidl;
	
	if ( NOERROR == SHGetMalloc( &pMalloc ) )
	{
		if ( NOERROR == SHGetSpecialFolderLocation( hWnd, nFolder, &pidl ) )
		{
			if ( SHGetPathFromIDList( pidl, Path ) )
			{
				pMalloc->Free( pidl );
				pMalloc->Release();
				return TRUE;
			}
			pMalloc->Free( pidl );
		}
		pMalloc->Release();
	}
	return FALSE;
}//GetSpecialFolder

BOOL GetSpecialFolderPath(HWND hWnd, int nFolder, tstring& tmpS)
{
	TCHAR szT[MAX_PATH]; szT[0] = 0;
	if (!GetSpecialFolderPath(hWnd, nFolder, szT))
		return FALSE;

	tmpS = szT;
	return TRUE;
}
