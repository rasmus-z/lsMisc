#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include <stdio.h>

#include "GetFormattedTime.h"

BOOL GetFormattedTime(LONGLONG llSec, LPTSTR szFormatted)
{
	if( szFormatted==NULL )
		return FALSE;


	UINT nHour = (UINT)llSec / 3600;
	llSec -= nHour*3600;
	assert(llSec < 3600);


	UINT nMinutes = (UINT)llSec / 60;
	llSec -= nMinutes*60;
	assert(llSec < 60);

	if( nHour > 99 )
		_stprintf(szFormatted, _T("??:%02d:%02I64u"), nMinutes, llSec);
	else
		_stprintf(szFormatted, _T("%02d:%02d:%02I64u"), nHour, nMinutes, llSec);

	return TRUE;
}
