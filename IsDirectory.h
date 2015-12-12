#pragma once
#include "CreateWString.h"


inline BOOL IsDirectoryW(LPCWSTR pDir)
{
	DWORD attr = GetFileAttributes(pDir);
	return (attr != 0xFFFFFFFF) && ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0);
}

inline BOOL IsDirectoryA(LPCSTR pDir)
{
	CREATEWSTRING(pDir);
	return IsDirectoryW(pDirW);
}

#ifdef UNICODE
#define IsDirectory IsDirectoryW
#else
#define IsDirectory IsDirectoryA
#endif