
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
		if(VerQueryValue(ver, "\\VarFileInfo\\Translation", (LPVOID*)&codepage, (PUINT)&len))
		{
			wsprintf(fmt, "\\StringFileInfo\\%04x%04x\\%s", (*codepage) & 0xFFFF, 
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

