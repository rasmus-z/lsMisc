#include "stdafx.h"
#include <windows.h>

#include "tstring.h"
#include "GetLastErrorString.h"
std::string GetLastErrorStringA(DWORD dwErrorNo, BOOL* pSeikou )
{
	LPVOID lpMsgBuf = NULL;
	std::string strRet;
 
	if( (0==FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL,
		dwErrorNo,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf,
		0,
		NULL)) || lpMsgBuf==NULL )
	{
		if(pSeikou)
		{
			*pSeikou = FALSE;
		}
		return strRet;
	}
 
	strRet = (LPSTR)lpMsgBuf;
	LocalFree(lpMsgBuf);
 
	if (pSeikou)
	{
		*pSeikou = TRUE;
	}
 
	return strRet;
}


std::wstring GetLastErrorStringW(DWORD dwErrorNo, BOOL* pSeikou )
{
	LPVOID lpMsgBuf = NULL;
	std::wstring strRet;
 
	if( (0==FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL,
		dwErrorNo,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf,
		0,
		NULL)) || lpMsgBuf==NULL )
	{
		if(pSeikou)
		{
			*pSeikou = FALSE;
		}
		return strRet;
	}
 
	strRet = (LPWSTR)lpMsgBuf;
	LocalFree(lpMsgBuf);
 
	if (pSeikou)
	{
		*pSeikou = TRUE;
	}
 
	return strRet;
}