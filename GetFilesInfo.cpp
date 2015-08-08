#include <Windows.h>
#include <shlwapi.h>

#include <vector>
#include <string>
#include <algorithm>

#include "tstring.h"

#include "GetFilesInfo.h"
BOOL GetFilesInfoW(LPCWSTR pDirectory, FILESINFOW& ret)
{
	LPTSTR pD = (LPTSTR)malloc( (lstrlen(pDirectory)+4)*sizeof(TCHAR));
	lstrcpy(pD, pDirectory);
	PathAddBackslash(pD);
	lstrcat(pD, L"*.*");

	myWIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile ( pD, &wfd);
	if(hFind == INVALID_HANDLE_VALUE )
		return FALSE;
	
	ret.directry_ = pDirectory;
	do
	{
		if (wfd.cFileName[0] != L'.')
		{
			ret.wfds_.push_back(wfd);
		}
	} while(FindNextFile (hFind, &wfd));
	FindClose (hFind);

	free(pD);
	return TRUE;
}

