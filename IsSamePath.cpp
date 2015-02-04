#include <windows.h>

#include "IsSamePath.h"

static BOOL isnullpath(LPCTSTR p)
{
	return p==NULL || *p==0;
}

static LPCTSTR getLongPath(LPCTSTR p)
{
	DWORD size = GetLongPathName(p, NULL, 0);
	if(size==0)
		return NULL;

	LPTSTR pBuff = (LPTSTR)malloc( (1+size) *sizeof(TCHAR));
	DWORD retsize = GetLongPathName(p, pBuff, 1+size);
	if(size >= retsize)
		return pBuff;

	free((void*)pBuff);
	return NULL;
}

BOOL IsSamePath(LPCTSTR p1, LPCTSTR p2)
{
	if(!isnullpath(p1) && !isnullpath(p2))
	{
		LPCTSTR p1L = getLongPath(p1);
		LPCTSTR p2L = getLongPath(p2);

		BOOL ret = lstrcmpi(p1L,p2L)==0;
		free((void*)p1L);
		free((void*)p2L);

		return ret;
	}
	return FALSE;
}