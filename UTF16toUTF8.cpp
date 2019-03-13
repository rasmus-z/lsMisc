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

#include <memory>
#include <string>
#include "UTF16toUTF8.h"

using namespace std;
//#define malloc noooooooooooomalloc
//#define free noooooooooooofree

namespace Ambiesoft {

	char* UTF16toMultiByte_new(UINT cp, LPCWSTR pIN, int inLen, int* pOutLen)
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


		char* pOut = new char[nReqSize + 1]; // (char*)malloc(nReqSize + 1);
		int nRet = WideCharToMultiByte(cp,
			0,
			pIN,
			inLen,
			pOut,
			nReqSize,
			NULL,
			NULL);

		if ((nRet != nReqSize) || (nRet == 0 && inLen != 0))
		{
			// free(pOut);
			delete[] pOut;
			if (pOutLen)
				*pOutLen = 0;
			return NULL;
		}

		pOut[nRet] = 0;

		if (pOutLen)
			*pOutLen = nRet;
		return pOut;

	}
	char* UTF16toUTF8_new(LPCWSTR pIN, int inLen, int* pOutLen)
	{
		return UTF16toMultiByte_new(CP_UTF8, pIN, inLen, pOutLen);
	}


	LPWSTR UTF16_convertEndian_new(LPCWSTR pIN)
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




	LPWSTR MultiBytetoUTF16_new(UINT cp, LPCSTR pIN, int inByteLen, int* pOutLen)
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
			pIN,
			inLen,
			NULL,
			0);

		LPWSTR pOut = new wchar_t[nReqSize + 1];  // (LPWSTR)malloc((nReqSize + 1)*sizeof(WCHAR));
		int nRet = MultiByteToWideChar(cp,
			0,
			pIN,
			inLen,
			pOut,
			nReqSize);

		if ((nRet != nReqSize) || (nRet == 0 && inLen != 0))
		{
			// free(pOut);
			delete[] pOut;
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

	LPWSTR UTF8toUTF16_new(const char* pIN, int inByteLen, int* pOutLen)
	{
		return MultiBytetoUTF16_new(CP_UTF8, pIN, inByteLen, pOutLen);
	}

	string toStdString(int acp, const wstring& w)
	{
		// BYTE* p = UTF16toUTF8(w.c_str(), (int)w.size());
		// char* p = UTF16toMultiByte(acp, w.c_str(), (int)w.size());
		unique_ptr<char> p(UTF16toMultiByte_new(acp, w.c_str(), (int)w.size()));
		if (!p)
			return string();
		//string ret = (char* )p;
		//free((void*)p);
		return p.get();
	}
	std::string toStdUtf8String(const std::wstring& w)
	{
		return toStdString(CP_UTF8, w);
	}
	std::string toStdAcpString(const std::wstring& w)
	{
		return toStdString(CP_ACP, w);
	}


	wstring toStdWstring(int acp, const char* pString, int inByteLen)
	{
		// LPCWSTR p = MultiBytetoUTF16(acp, pString, inByteLen);
		unique_ptr<wchar_t> p(MultiBytetoUTF16_new(acp, pString, inByteLen));
		if (!p)
			return wstring();
		//wstring ret = p;
		//free((void*)p);
		return p.get();
	}



	std::wstring toStdWstringFromUtf8(const char* pUtf8, int inByteLen)
	{
		return toStdWstring(CP_UTF8, pUtf8, inByteLen);
	}
	wstring toStdWstringFromUtf8(const string& s)
	{
		return toStdWstringFromUtf8(s.c_str(), (int)s.size());
	}

	std::wstring toStdWstringFromACP(const char* pACP, int inByteLen)
	{
		return toStdWstring(CP_ACP, pACP, inByteLen);
	}
	wstring toStdWstringFromACP(const string& s)
	{
		return toStdWstringFromACP(s.c_str(), (int)s.size());
	}


#ifdef __cplusplus_cli  
	char* UTF16toUTF8_new(System::String^ s)
	{
		pin_ptr<const wchar_t> p = PtrToStringChars(s);
		return UTF16toUTF8_new(p);
	}
#endif //__cplusplus_cli  


} // namespace