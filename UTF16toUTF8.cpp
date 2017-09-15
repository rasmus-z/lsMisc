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

namespace Ambiesoft {

BYTE* UTF16toMultiByte(UINT cp, LPCWSTR pIN, size_t* pLenOut)
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

	if(pLenOut)
		*pLenOut = nRet;
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




LPWSTR MultiBytetoUTF16(UINT cp, const LPBYTE pIN, size_t cbLen)
{
	int nReqSize = MultiByteToWideChar(
		cp,
		0,
		(const char*)pIN,
		(int)cbLen,
		NULL,
		0);

	if ( nReqSize == 0 )
		return NULL;

	LPWSTR pOut = (LPWSTR)malloc(nReqSize*sizeof(WCHAR));
	int nRet = MultiByteToWideChar(cp,
		0,
		(const char*)pIN,
		(int)cbLen,
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


#ifdef __cplusplus_cli  
BYTE* UTF16toUTF8(System::String^ s)
{
	pin_ptr<const wchar_t> p = PtrToStringChars(s);
	return UTF16toUTF8(p);
}
#endif //__cplusplus_cli  


} // namespace