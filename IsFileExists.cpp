#include "stdafx.h"
#include <windows.h>
#include "IsFileExists.h"
BOOL IsFileExistsW(LPCWSTR pPath)
{
	DWORD dwTempAttr = ::GetFileAttributesW(pPath);
	return ( dwTempAttr != 0xFFFFFFFF && !(dwTempAttr & FILE_ATTRIBUTE_DIRECTORY) );
}
BOOL IsFileExistsA(LPCSTR pPath)
{
	DWORD dwTempAttr = ::GetFileAttributesA(pPath);
	return ( dwTempAttr != 0xFFFFFFFF && !(dwTempAttr & FILE_ATTRIBUTE_DIRECTORY) );
}