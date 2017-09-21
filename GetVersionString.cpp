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

#include "tchar.h"
#include "GetVersionString.h"

#pragma comment(lib, "Version.lib")
#pragma comment(lib, "User32.lib")
//
//	Get the specified file-version information string from a file
//	
//	szItem	- version item string, e.g:
//		"FileDescription", "FileVersion", "InternalName", 
//		"ProductName", "ProductVersion", etc  (see MSDN for others)
//
TCHAR *GetVersionString(TCHAR *szFileName, TCHAR *szValue, TCHAR *szBuffer, ULONG nLength)
{
	DWORD  len;
	PVOID  ver;	
	DWORD  *codepage;
	TCHAR   fmt[0x40];
	PVOID  ptr = 0;
	BOOL   result = FALSE;
	
	szBuffer[0] = '\0';

	len = GetFileVersionInfoSize(szFileName, 0);

	if(len == 0 || (ver = malloc(len)) == 0)
		return NULL;

	if(GetFileVersionInfo(szFileName, 0, len, ver))
	{
		if(VerQueryValue(ver, _T("\\VarFileInfo\\Translation"), (LPVOID*)&codepage, (PUINT)&len))
		{
			wsprintf(fmt, _T("\\StringFileInfo\\%04x%04x\\%s"), (*codepage) & 0xFFFF, 
					(*codepage) >> 16, szValue);
			
			if(VerQueryValue(ver, fmt, &ptr, (PUINT)&len))
			{
				lstrcpyn(szBuffer, (TCHAR*)ptr, min(nLength, len));
				result = TRUE;
			}
		}
	}

	free(ver);
	return result ? szBuffer : NULL;
}

