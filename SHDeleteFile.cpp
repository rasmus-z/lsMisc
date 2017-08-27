#include "StdAfx.h"
#include <windows.h>
#include <Shellapi.h>
#include <tchar.h>

#include <assert.h>
#include <malloc.h>

#include "SHDeleteFile.h"

BOOL SHDeleteFile(LPCTSTR lpFile, SHDELETEFLAG sdf)
{
	size_t len = _tcslen(lpFile);
	if(!lpFile || lpFile[0]==0 || len <= 3)
		return FALSE;

	do
	{
#ifndef UNICODE
		if (IsDBCSLeadByte((BYTE)lpFile[0]))
			return FALSE;
#endif

		if( lpFile[0]==_T('\\') && lpFile[1]==_T('\\') )
			break;

		if( lpFile[1] == _T(':') && lpFile[2]==_T('\\') )
			break;

		return FALSE;
	} while(false);

	LPTSTR p = (LPTSTR)_alloca( (len+2)*sizeof(TCHAR) );
	if(!p)
		return FALSE;
	lstrcpy(p, lpFile);
	p[len+1]=0;

	SHFILEOPSTRUCT sfo = {0};
	sfo.hwnd = NULL;
	sfo.wFunc = FO_DELETE;
	sfo.pFrom = p;
	sfo.pTo   = NULL;  // ignored
	sfo.fFlags = 0;
	if(sdf & SHDELETE_COMPLETEDELETE)
		sfo.fFlags |= FOF_NOCONFIRMATION;
	else
		sfo.fFlags |= FOF_ALLOWUNDO;
	
	if(sdf & SHDELETE_NOUI)
		sfo.fFlags |=  (FOF_SILENT|FOF_NOERRORUI);

	int ret = SHFileOperation(&sfo);
	return ret==0;
}
