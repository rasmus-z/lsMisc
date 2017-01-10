#include "stdafx.h"
#include <windows.h>
#include <string>
#include "UTF16toUTF8.h"


BYTE* UTF16toMultiByte(UINT cp, LPCWSTR pIN)
{
	int nReqSize = WideCharToMultiByte(cp,
		0,
		pIN,
		-1,
		NULL,
		0,
		NULL,
		NULL);

	if ( nReqSize == 0 )
		return NULL;

	BYTE* pOut = (BYTE*)malloc(nReqSize);
	int nRet = WideCharToMultiByte(cp,
		0,
		pIN,
		-1,
		(char*)pOut,
		nReqSize,
		NULL,
		NULL);

	if ( nRet==0 || nRet != nReqSize )
	{
		free(pOut);
		return NULL;
	}

	return pOut;

}
BYTE* UTF16toUTF8(LPCWSTR pIN)
{
	return UTF16toMultiByte(CP_UTF8, pIN);
}

LPWSTR UTF16_convertEndian(LPCWSTR pIN)
{
	if (pIN == NULL)
		return NULL;

#if 0
	LPWSTR p = wcsdup(pIN);
#else
	LPWSTR p = _wcsdup(pIN);
#endif
	LPWSTR pRet = p;
	for (; *pIN; ++pIN, ++p)
	{
		*p = ((*pIN & 0xFF) << 8) | ((*pIN & 0xFF00) >> 8);
	}
	*p = 0;
	return pRet;
}




LPWSTR MultiBytetoUTF16(UINT cp, const LPBYTE pIN)
{
	int nReqSize = MultiByteToWideChar(
		cp,
		0,
		(const char*)pIN,
		-1,
		NULL,
		0);

	if ( nReqSize == 0 )
		return NULL;

	LPWSTR pOut = (LPWSTR)malloc(nReqSize*sizeof(WCHAR));
	int nRet = MultiByteToWideChar(cp,
		0,
		(const char*)pIN,
		-1,
		pOut,
		nReqSize);

	if ( nRet==0 || nRet != nReqSize )
	{
		free(pOut);
		return NULL;
	}

	return pOut;
}

LPWSTR UTF8toUTF16(const LPBYTE pIN)
{
	return MultiBytetoUTF16(CP_UTF8, pIN);
}

bool UTF8toUTF16(const LPBYTE pIN, std::wstring& w)
{
	LPCWSTR pOut = UTF8toUTF16(pIN);
	if(!pOut)
		return false;

	w = pOut;
	free((void*)pOut);
	return true;
}
