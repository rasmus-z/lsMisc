#include <windows.h>
#include <tchar.h>


BOOL stringEndwith(LPCTSTR pString, LPCTSTR pEnd)
{
	if(!pString)
		return FALSE;

	if(!pEnd || pEnd[0]==0)
		return TRUE;

	size_t psize = lstrlen(pString);
	size_t endlen = lstrlen(pEnd);
	if(psize < endlen)
		return FALSE;

	LPCTSTR pS = pString;
	LPCTSTR pSS = pS + psize - endlen;
	while(*pEnd)
	{
		if(*pSS != *pEnd)
			return FALSE;

		++pSS;
		++pEnd;
	}
	return TRUE;
}

BOOL stringEndwithI(LPCTSTR pString, LPCTSTR pEnd)
{
	if(!pString)
		return FALSE;

	if(!pEnd || pEnd[0]==0)
		return TRUE;

	LPTSTR pdup = _tcsdup(pString);
	_tcslwr(pdup);

	LPTSTR endup = _tcsdup(pEnd);
	_tcslwr(endup);

	BOOL ret = stringEndwith(pdup, endup);
	free(pdup);
	free(endup);
	return ret;
}
