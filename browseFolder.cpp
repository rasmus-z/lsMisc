#include <windows.h>
#include <tchar.h>
#include <objbase.h>
#include <shlobj.h>

#include "browseFolder.h"


BOOL browseFolder( HWND hParent, LPCTSTR lpszTitle, LPTSTR pFolder )
{
	BOOL bRet = FALSE;


	TCHAR szDisplayName[_MAX_PATH];
	TCHAR szBuffer[_MAX_PATH];

	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = hParent;
	// set root at Desktop
	browseInfo.pidlRoot = NULL; 
	browseInfo.pszDisplayName = szDisplayName;
	browseInfo.lpszTitle = lpszTitle;
	browseInfo.ulFlags = BIF_RETURNONLYFSDIRS ;   // also passed in
	browseInfo.lpfn = NULL;
	browseInfo.lParam = 0;      // not used   

	LPITEMIDLIST lpItemIDList;
	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
	{
		// Get the path of the selected folder from the
		// item ID list.
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer) && szBuffer[0] != '\0' )
		{
			lstrcpy(pFolder, szBuffer);
			bRet = TRUE;
		}

		// lpMalloc->Free(lpItemIDList);
		CoTaskMemFree(lpItemIDList);
	}

	return bRet;
}


