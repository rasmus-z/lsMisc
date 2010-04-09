#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include "RemoveLastInValidURLChar.h"

BOOL RemoveLastInValidURLChar(LPTSTR lpString)
{
	if( lpString == NULL || _tcslen(lpString)==0 )
		return FALSE;

	LPTSTR p = lpString + _tcslen(lpString);	
	assert(*p==_T('\0'));
	
	LPTSTR pOrg = lpString;

	p = _tcsdec(pOrg, p);
	while( p >= lpString ) {
		if( !(*p == _T('/') || *p == _T('.') || *p == _T(':') ||
			(*p >= _T('a') && *p <= _T('z')) || (*p >= _T('A') && *p <= _T('Z')) || (*p >= _T('0') && *p <= _T('9')) ||
			*p == _T('@') || *p == _T('&') || *p == _T('=') || *p == _T('+') || *p == _T('$') || *p == _T(',') ||
			*p == _T('-') || *p == _T('_') || *p == _T('!') || *p == _T('~') || *p == _T('*') || *p == _T('\'') ||
			*p == _T('(') || *p == _T(')') || *p == _T('?') || *p == _T(';') || *p == _T('%')) ) 
		{
			*p = 0;
		}
		p = _tcsdec(pOrg, p);
	};

	return TRUE;
}