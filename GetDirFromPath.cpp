#include "stdafx.h"

#ifndef _WINDOWS_
#pragma message("including windows.h")
#include <windows.h>
#endif

#include "tstring.h"

#include <malloc.h>
#include <shlwapi.h>
#include <tchar.h>

#include "GetDirFromPath.h"
#pragma comment(lib,"shlwapi.lib")


tstring GetDirFromPath(LPCTSTR pszPathName)
{
	LPTSTR p = (LPTSTR)_alloca( (lstrlen(pszPathName) + 1) * sizeof(TCHAR));
	lstrcpy(p, pszPathName);
	DWORD dwAttributes = ::GetFileAttributes(p);
	if(dwAttributes != 0xFFFFFFFF && (dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
	{// this is already dir.
		PathAddBackslash(p);
		return p;
	}

	LPTSTR pSep = _tcsrchr(p, _T('\\'));
	if( pSep==NULL )
	{// illegal path
		pSep = _tcsrchr(p, _T('/'));
		if(pSep==NULL)
			return _T("");
	}
	
	*(pSep+1) = 0;
	return p;
}
