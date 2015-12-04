#include "stdafx.h"
#include <windows.h>
#include "IsFileExists.h"
#include "CreateWString.h"

BOOL IsFileExistsW(LPCWSTR pPath)
{
	DWORD dwTempAttr = ::GetFileAttributesW(pPath);
	return ( dwTempAttr != 0xFFFFFFFF && !(dwTempAttr & FILE_ATTRIBUTE_DIRECTORY) );
}
BOOL IsFileExistsA(LPCSTR pPath)
{
	CREATEWSTRING(pPath);
	return IsFileExistsW(pPathW);
}