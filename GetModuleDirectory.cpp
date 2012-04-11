#include <windows.h>
#include <tchar.h>
#include "tstring.h"
#include "GetModuleDirectory.h"
#include "GetDirFromPath.h"
#include "HelpDefines.h"

BOOL GetModuleDirectory(HINSTANCE hInstance, LPTSTR pDir, DWORD dwCount)
{
	if ( !GetModuleFileName(hInstance, pDir, dwCount) )
		return FALSE;

	tstring ret = GetDirFromPath(pDir);
	if ( ret.empty() )
		return FALSE;

	lstrcpy(pDir, ret.c_str());
	return TRUE;
}

tstring GetModuleDirectory(HINSTANCE hInstance)
{
	TCHAR szT[MAX_PATH];
	if(!GetModuleDirectory(hInstance, szT, _countof(szT)))
		return _T("");

	return szT;
}