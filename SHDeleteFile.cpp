//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

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
