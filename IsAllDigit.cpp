#include <windows.h>
#include <tchar.h>

#include "IsAllDigit.h"

BOOL IsAllDigit(LPCTSTR lpText)
{
	if( lpText==NULL )
		return FALSE;

	for(; *lpText ; lpText++) {
		if( !( _T('0') <= *lpText && *lpText <= _T('9') ) )
			return FALSE;
	}
	return TRUE;
}