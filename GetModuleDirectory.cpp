#include <windows.h>
#include "tstring.h"
#include "GetModuleDirectory.h"
#include "GetDirFromPath.h"

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


