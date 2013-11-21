#include <windows.h>

#include "tstring.h"
#include "GetLastErrorString.h"
tstring GetLastErrorString(DWORD dwErrorNo, BOOL* pSeikou )
{
	LPVOID lpMsgBuf = NULL;
	tstring strRet;
 
	if( (0==FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL,
		dwErrorNo,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL)) || lpMsgBuf==NULL )
	{
		if(pSeikou)
		{
			*pSeikou = FALSE;
		}
		return strRet;
	}
 
	strRet = (LPTSTR)lpMsgBuf;
	LocalFree(lpMsgBuf);
 
	if (pSeikou)
	{
		*pSeikou = TRUE;
	}
 
	return strRet;
}