#include <windows.h>
#include <tchar.h>
#include <assert.h>

#include "SHDeleteFile.h"

BOOL SHDeleteFile(LPCTSTR lpFile)
{
	TCHAR szT[MAX_PATH];
	lstrcpy(szT, lpFile);
	assert(szT[lstrlen(szT)] == 0);
	szT[lstrlen(szT)+1] = 0;

	SHFILEOPSTRUCT sfo = {0};
	sfo.hwnd = NULL;
	sfo.wFunc = FO_DELETE;
	sfo.pFrom = szT;
	sfo.pTo   = NULL;  // ignored
	sfo.fFlags = FOF_ALLOWUNDO|FOF_FILESONLY;

	return 0==SHFileOperation(&sfo);
}
