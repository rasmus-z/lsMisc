#include <windows.h>
#include <tchar.h>
#include <string.h>

#include "myStrStrI.h"

static TCHAR myToLowSimple(TCHAR t)
{
	if ( TEXT('A') <= t && t <= TEXT('Z') )
		return t - TEXT('A') + TEXT('a');

	return t;
}


static LPTSTR myStrChrI(
	LPCTSTR lpStart,
	TCHAR wMatch
)
{
	if ( lpStart==NULL || *lpStart == 0 )
		return NULL;

	const size_t length = _tcslen(lpStart);
	for ( UINT i=0 ; i < length; ++i )
	{
		TCHAR t = myToLowSimple(lpStart[i]);
		TCHAR w = myToLowSimple(wMatch);

		if ( t == w )
			return (LPTSTR)&lpStart[i];
	}
	return NULL;
}

LPTSTR myStrStrI(
	LPCTSTR lpFirst,
	LPCTSTR lpSrch
)
{
	if ( lpFirst==NULL || *lpFirst==0 || lpSrch==NULL || *lpSrch==0 )
	{
		return (LPTSTR)lpFirst;
	}

	const size_t length = _tcslen(lpSrch);

	if ( length > _tcslen(lpFirst) )
		return NULL;

	for( LPCTSTR p = lpFirst; NULL != (p = myStrChrI(p, lpSrch[0])); p++ )
	{
		if ( !_tcsncicmp( p, lpSrch, length ) )
		{
			return (LPTSTR)p;
		}
	}

	return NULL;
}
