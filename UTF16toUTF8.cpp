//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#include "stdafx.h"
#include <windows.h>

#ifdef __cplusplus_cli  
#include <vcclr.h>
#endif

#include <string>
#include "UTF16toUTF8.h"

using namespace std;

namespace Ambiesoft {

BYTE* UTF16toMultiByte(UINT cp, LPCWSTR pIN, int inLen, int* pOutLen)
{
	if (pIN == nullptr)
	{
		if (pOutLen)
			*pOutLen = 0;
		return nullptr;
	}

	if (inLen == -1)
	{
		inLen = (int)wcslen(pIN);
	}
	int nReqSize = WideCharToMultiByte(cp,
		0,
		pIN,
		inLen,
		NULL,
		0,
		NULL,
		NULL);


	BYTE* pOut = (BYTE*)malloc(nReqSize + 1);
	int nRet = WideCharToMultiByte(cp,
		0,
		pIN,
		inLen,
		(char*)pOut,
		nReqSize,
		NULL,
		NULL);

	if ((nRet != nReqSize) || (nRet == 0 && inLen != 0))
	{
		free(pOut);
		if (pOutLen)
			*pOutLen = 0;
		return NULL;
	}

	pOut[nRet] = 0;

	if(pOutLen)
		*pOutLen = nRet;
	return pOut;

}
BYTE* UTF16toUTF8(LPCWSTR pIN, int inLen, int* pOutLen)
{
	return UTF16toMultiByte(CP_UTF8, pIN, inLen, pOutLen);
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




LPWSTR MultiBytetoUTF16(UINT cp, LPCSTR pIN, int inByteLen, int* pOutLen)
{
	if (pIN == nullptr)
	{
		if (pOutLen)
			*pOutLen = 0;
		return nullptr;
	}

	int inLen = inByteLen;
	if (inLen == -1)
	{
		inLen = (int)strlen(pIN);
	}

	int nReqSize = MultiByteToWideChar(
		cp,
		0,
		(const char*)pIN,
		inLen,
		NULL,
		0);

	LPWSTR pOut = (LPWSTR)malloc((nReqSize+1)*sizeof(WCHAR));
	int nRet = MultiByteToWideChar(cp,
		0,
		(const char*)pIN,
		inLen,
		pOut,
		nReqSize);

	if ( (nRet != nReqSize) || (nRet==0 && inLen != 0))
	{
		free(pOut);
		if (pOutLen)
			*pOutLen = 0;
		return NULL;
	}

	pOut[nRet] = 0;

	if (pOutLen)
	{
		*pOutLen = nRet;
	}
	return pOut;
}

LPWSTR UTF8toUTF16(const LPBYTE pIN, int inByteLen, int* pOutLen)
{
	return MultiBytetoUTF16(CP_UTF8, (LPCSTR)pIN, inByteLen, pOutLen);
}

//bool UTF8toUTF16(const LPBYTE pIN, std::wstring& w)
//{
//	return MultiBytetoUTF16(CP_UTF8, (LPCSTR)pIN, w);
//}

//bool MultiBytetoUTF16(UINT cp, LPCSTR pIN, std::wstring& wstr)
//{
//	LPCWSTR pOut = MultiBytetoUTF16(cp, pIN);
//	if (!pOut)
//		return false;
//
//	wstr = pOut;
//	free((void*)pOut);
//	return true;
//}

string toStdString(const wstring& w)
{
	BYTE* p = UTF16toUTF8(w.c_str(), (int)w.size());
	string ret = (char* )p;
	free((void*)p);
	return ret;
}

wstring toStdWstringFromUtf8(const string& s)
{
	return toStdWstringFromUtf8(s.c_str(), (int)s.size());
}
std::wstring toStdWstringFromUtf8(const char* pUtf8, int inByteLen)
{
	wstring ret;
	LPCWSTR p = UTF8toUTF16((const LPBYTE)pUtf8, inByteLen);
	ret = p;
	free((void*)p);
	return ret;
}

#ifdef __cplusplus_cli  
BYTE* UTF16toUTF8(System::String^ s)
{
	pin_ptr<const wchar_t> p = PtrToStringChars(s);
	return UTF16toUTF8(p);
}
#endif //__cplusplus_cli  


} // namespace