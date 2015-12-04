#include "StdAfx.h"
#include <windows.h>
#include "IsFileOpen.h"


ISFILEOPEN IsFileOpen(LPCTSTR pFile)
{
	HANDLE f = CreateFile(
		pFile,
		GENERIC_READ,
		0, // share
		NULL, // sec
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if(f==INVALID_HANDLE_VALUE)
	{
		switch(GetLastError())
		{
		case ERROR_SHARING_VIOLATION:return ISFILEOPEN_YES;
		case ERROR_ACCESS_DENIED:return ISFILEOPEN_UNKNOWN;
		}
		return ISFILEOPEN_NO;
	}

	CloseHandle(f);
	return ISFILEOPEN_NO;
}
