#include <windows.h>
#include "IsValidString.h"

BOOL IsValidString(LPCTSTR lpsz, int nLength)
{
	if (lpsz == NULL)
		return FALSE;
	return ::IsBadStringPtr(lpsz, nLength) == 0;
}
