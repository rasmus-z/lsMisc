#include <windows.h>
#include <tchar.h>

#include "ChangeFilenamable.h"

BOOL ChangeFilenamable(LPTSTR pch)
{
	TCHAR* pTemp = pch;
	while( *pch != '\0' )
	{
		TCHAR* pOrgch = pch;
		if( *pch==_T('\\')
		||  *pch==_T(':')
		||  *pch==_T('*')
		||  *pch==_T('?')
		||  *pch==_T('\"')
		||  *pch==_T('<')
		||  *pch==_T('>')
		||  *pch==_T('|') )
		{
			do
			{
				*pOrgch = *(pOrgch+1);
				++pOrgch;
			}
			while( *pOrgch );
		}
		else
		{
			pch = (TCHAR*)_tcsinc(pch);
		}
	}
	
	pch = pTemp;
	while( *pch != _T('\0') )
	{
		if( *pch == _T('/') )
			*pch = _T('-');

		pch = (TCHAR*)_tcsinc(pch);
	}

	return TRUE;
}