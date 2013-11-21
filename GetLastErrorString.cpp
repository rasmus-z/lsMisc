#include <windows.h>

#include "tstring.h"
// LPCTSTR GetLastErrorString(AString& strError, DWORD dwErrorNo)
LPCTSTR GetLastErrorString(tstring& strError, DWORD dwErrorNo)
{
	LPVOID lpMsgBuf = NULL;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL,
		dwErrorNo,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL
	);


	return lpMsgBuf;
}
