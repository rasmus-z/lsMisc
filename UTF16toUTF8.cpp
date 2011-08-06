#include <windows.h>
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


LPCWSTR UTF8toUTF16(LPBYTE pIN)
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

	LPWSTR pOut = (LPWSTR)malloc(nReqSize);
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
