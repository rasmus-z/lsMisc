#include <windows.h>
#include "UTF16toUTF8.h"
#include "UrlEncode2.h"

BYTE* UTF16toUTF8(LPCWSTR pIN, BOOL bUrlEncode)
{
	int nReqSize = WideCharToMultiByte( CP_UTF8,
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
	int nRet = WideCharToMultiByte( CP_UTF8,
		0,
		pIN,
		-1,
		(char*)pOut,
		nReqSize,
		NULL,
		NULL);

	if ( nRet==0 || nRet != nReqSize )
		return NULL;

	if ( bUrlEncode )
	{
		int nEncodeSize = nRet*3;
		BYTE* pOut2 = NULL;
		UrlEncode2(pOut, lstrlen((char*)pOut), (char**)&pOut2, 1);
		free(pOut);
		return pOut2;
	}
	return pOut;;
}
