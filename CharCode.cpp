#include <tchar.h>
#include <windows.h>
#include <comdef.h>

#include "SJISToEUC.h"
#include "UnicodeToUTF8.h"
#include "URLEncode2.h"
#include "typehelp.h"

void ChangeCharcode(ENCODE_TYPE type, _bstr_t& bstrOut)
{
	if(type==ENCODE_NONE)
		return;
	BYTE* pDataCharCoded = NULL;
#ifdef I18N_ENGLISH
	if(type==ENCODE_SYSDEFAULT)
	{
	}
	else if(type==ENCODE_ISO_8859_1)
	{
	}
#else
	if(type==ENCODE_SJIS || type==ENCODE_EUC)
	{
		UINT nSizeOrig = lstrlen((LPCTSTR)bstrOut);
		BYTE* pData = new BYTE[nSizeOrig+1];
		if(pData==NULL) return;

		ZeroMemory(pData, nSizeOrig+1);
		memcpy(pData, (LPCTSTR)bstrOut, nSizeOrig);
		
		if(type==ENCODE_EUC)
		{
			SJISToEUC((LPCSTR)pData, &pDataCharCoded);
			delete [] pData;
		}
		else
		{
			pDataCharCoded = pData;
			pData = NULL;
		}

		LPTSTR pURLEncoded = NULL;
		UrlEncode2(pDataCharCoded, nSizeOrig, &pURLEncoded);
		delete[] pDataCharCoded;

		bstrOut = pURLEncoded;
		delete[] pURLEncoded;
	}
#endif
	else if(type==ENCODE_UTF8)
	{
		UnicodeToUTF8((BYTE*)(BSTR)bstrOut, &pDataCharCoded);

		char* pURLEncoded = NULL;
		UrlEncode2(pDataCharCoded, lstrlen((const char*)pDataCharCoded),
			&pURLEncoded);
		bstrOut = pURLEncoded;
		delete[] pURLEncoded;
		
		delete[] pDataCharCoded;
		pDataCharCoded = NULL;
	}
}