#include <windows.h>
#include <tchar.h>
#include <shlobj.h>
#include "GetSpecialFolderPath.h"

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
