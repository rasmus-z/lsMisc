#include "stdafx.h"

#include <windows.h>
#include <tchar.h>
#include <assert.h>

#include "SHMoveFile.h"

BOOL SHMoveFile(LPCTSTR lpFileTo, LPCTSTR lpFileFrom)
{
	assert(lpFileTo && lpFileFrom);

	if( GetFileAttributes(lpFileFrom)==0xFFFFFFFF )
		return FALSE;

	TCHAR szTo[MAX_PATH];
	TCHAR szFrom[MAX_PATH];
	lstrcpy(szTo, lpFileTo);
	lstrcpy(szFrom, lpFileFrom);

	szTo[lstrlen(szTo)+1] = 0;
	szFrom[lstrlen(szFrom)+1] = 0;

	SHFILEOPSTRUCT sfo = {0};
	sfo.hwnd = NULL;
	sfo.wFunc = FO_MOVE;
	sfo.pFrom = szFrom;
	sfo.pTo   = szTo;
	sfo.fFlags = FOF_ALLOWUNDO|FOF_FILESONLY|FOF_NOERRORUI;

	int nRet = SHFileOperation(&sfo) ;//&& !sfo.fAnyOperationsAborted;

	// it seems SHFileOperation returns 113 when same path specified.
	return nRet==0 || nRet==113;
}
