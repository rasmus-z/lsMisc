#include "stdafx.h"
#include <windows.h>
#include <string>
#include "UTF16toUTF8.h"

BYTE* UTF16toUTF8(LPCWSTR pIN)
{
	int nReqSize = WideCharToMultiByte(CP_UTF8,
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
	int nRet = WideCharToMultiByte(CP_UTF8,
		0,
		pIN,
		-1,
		(char*)pOut,
		nReqSize,
		NULL,
		NULL);

	if ( nRet==0 || nRet != nReqSize )
		return NULL;

	return pOut;;
}

LPWSTR UTF16_convertEndian(LPCWSTR pIN)
{
	if (pIN == NULL)
		return NULL;

	LPWSTR p = wcsdup(pIN);
	LPWSTR pRet = p;
	for (; *pIN; ++pIN, ++p)
	{
		*p = ((*pIN & 0xFF) << 8) | ((*pIN & 0xFF00) >> 8);
	}
	*p = 0;
	return pRet;
}

LPWSTR UTF8toUTF16(const LPBYTE pIN)
{
	
	int nReqSize = MultiByteToWideChar(
		CP_UTF8,
		0,
		(const char*)pIN,
		-1,
		NULL,
		0);

	if ( nReqSize == 0 )
		return NULL;

	LPWSTR pOut = (LPWSTR)malloc(nReqSize*sizeof(WCHAR));
	int nRet = MultiByteToWideChar(CP_UTF8,
		0,
		(const char*)pIN,
		-1,
		pOut,
		nReqSize);

	if ( nRet==0 || nRet != nReqSize )
		return NULL;

/**
	if ( bUrlEncode )
	{
		int nEncodeSize = nRet*3;
		BYTE* pOut2 = NULL;
		UrlEncode2(pOut, lstrlenA((char*)pOut), (char**)&pOut2, 1);
		free(pOut);
		return pOut2;
	}
**/
	return pOut;;
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
