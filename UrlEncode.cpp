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
#include <Windows.h>

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <string>

#ifdef _DEBUG
#include <cassert>
#endif

#include <stlsoft/smartptr/scoped_handle.hpp>

#include "UTF16toUTF8.h"
#include "UrlEncode.h"

using namespace std;


namespace Ambiesoft {

//static bool myiswalnum(wchar_t c)
//{
//	switch (c) {
//	case L'0': return true;
//	case L'1': return true;
//	case L'2': return true;
//	case L'3': return true;
//	case L'4': return true;
//	case L'5': return true;
//	case L'6': return true;
//	case L'7': return true;
//	case L'8': return true;
//	case L'9': return true;
//	case L'a':case L'A': return true;
//	case L'b':case L'B': return true;
//	case L'c':case L'C': return true;
//	case L'd':case L'D': return true;
//	case L'e':case L'E': return true;
//	case L'f':case L'F': return true;
//	case L'g':case L'G': return true;
//	case L'h':case L'H': return true;
//	case L'i':case L'I': return true;
//	case L'j':case L'J': return true;
//	case L'k':case L'K': return true;
//	case L'l':case L'L': return true;
//	case L'm':case L'M': return true;
//	case L'n':case L'N': return true;
//	case L'o':case L'O': return true;
//	case L'p':case L'P': return true;
//	case L'q':case L'Q': return true;
//	case L'r':case L'R': return true;
//	case L's':case L'S': return true;
//	case L't':case L'T': return true;
//	case L'u':case L'U': return true;
//	case L'v':case L'V': return true;
//	case L'w':case L'W': return true;
//	case L'x':case L'X': return true;
//	case L'y':case L'Y': return true;
//	case L'z':case L'Z': return true;
//	}
//	return false;
//}



// http://www.emoticode.net/c/urlencode-in-plain-c.html
static char i2aA(char code) {
	static char hex[] = "0123456789ABCDEF";
	return hex[code & 15];
}
//static wchar_t i2aW(wchar_t code) {
//	static wchar_t hex[] = L"0123456789ABCDEF";
//	return hex[code & 15];
//}


char *UrlEncode(const unsigned char *pstr, size_t size)
{
	char
		*buf,
		*pbuf;

	if (pstr == NULL)
		return NULL;

	if (size == -1)
		size = strlen((const char*)pstr);

	if (size == 0)
	{
		return (char*)calloc(1, 1);
	}

	pbuf = buf = (char *)malloc(size * 3 + 1);

	while (size--) {
		if ((*pstr >0 && isalnum(*pstr)) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') {
			*pbuf++ = *pstr;
		}
		else if (*pstr == ' ') {
			*pbuf++ = '+';
		}
		else {
			*pbuf++ = '%';
			*pbuf++ = i2aA( (*pstr & 0xFF) >> 4);
			*pbuf++ = i2aA(*pstr & 0xF);
		}
		pstr++;
	}
	*pbuf = '\0';

	return buf;
}
std::wstring UrlEncodeW(const wchar_t *pstr)
{
	std::wstring ret;
	if (pstr == NULL || pstr[0] == 0)
		return ret;

	BYTE* p8 = UTF16toUTF8(pstr);
	stlsoft::scoped_handle<void*> ma(p8, free);

	char* pRet8 = UrlEncode(p8);
	stlsoft::scoped_handle<void*> ma2(pRet8, free);

	UTF8toUTF16((BYTE*)pRet8, ret);
	return ret;
}




static char a2ibyte(char c)
{
	switch (c) {
	case '0': return 0x0;
	case '1': return 0x1;
	case '2': return 0x2;
	case '3': return 0x3;
	case '4': return 0x4;
	case '5': return 0x5;
	case '6': return 0x6;
	case '7': return 0x7;
	case '8': return 0x8;
	case '9': return 0x9;
	case 'a':case 'A': return 0xa;
	case 'b':case 'B': return 0xb;
	case 'c':case 'C': return 0xc;
	case 'd':case 'D': return 0xd;
	case 'e':case 'E': return 0xe;
	case 'f':case 'F': return 0xf;
	}
	return 0x10;
}
//static char a2iA(char c1, char c2)
//{
//	return (a2ibyte(c1) << 4) | (a2ibyte(c2));
//}
//static WCHAR a2iW(WCHAR c1, WCHAR c2)
//{
//	return (a2ibyte(c1) << 4) | (a2ibyte(c2));
//}

//static bool IsPropertyEncodedPercent(LPCSTR p)
//{
//	if (!p || p[0] == 0)
//		return false;
//
//	char c1 = *p;
//	++p;
//	if (!p || p[0] == 0)
//		return false;
//
//	char c2 = *p;
//
//	if ('0')
//}
unsigned char* UrlDecode(const char* penc, size_t* psize)
{
	unsigned int size = 0;
	unsigned char* pOrig = (unsigned char*)malloc(strlen(penc) + 1);
	unsigned char* p = pOrig;
	for (; *penc; ++penc, ++p, ++size)
	{
		char c = *penc;
		if (c == '%')
		{
			const char* pSave = penc;
			penc++;
			if (!*penc)
			{
				break;
			}
			char c1, c2;
			c1 = *penc;
			penc++;
			if (!*penc)
			{
				*p = c1;
				break;
			}
			c2 = *penc;

			char c1con = a2ibyte(c1);
			char c2con = a2ibyte(c2);
			if (c1con == 0x10 || c2con == 0x10)
			{
				//failed conversion
				penc = pSave;
				*p = *penc;
				continue;
			}

			c = ((c1con << 4) | c2con);
			//c = a2i(c1, c2);
		}
		else if (c == '+')
		{
			c = ' ';
		}
		*p = c;
	}
	*p = 0;
	if (psize)
		*psize = size;
	return pOrig;
}

#ifdef _DEBUG
static void checkmoji()
{
	assert('1' == L'1');
	assert('1' != L'2');
	assert('a' == L'a');
}
#endif


std::wstring UrlDecodeW(const char* penc)
{
	std::wstring ret;
	BYTE* p8 = UrlDecode(penc);
	stlsoft::scoped_handle<void*> ma(p8, free);
	UTF8toUTF16(p8, ret);
	return ret;
}

std::wstring UrlDecodeW(const std::wstring& wenc)
{
	BYTE* p8 = UTF16toUTF8(wenc.c_str());
	stlsoft::scoped_handle<void*> ma(p8, free);

	BYTE* p8dec = UrlDecode((LPCSTR)p8);
	stlsoft::scoped_handle<void*> map8dec(p8dec, free);

	std::wstring ret;
	UTF8toUTF16(p8dec, ret);
	return ret;
}

} // namespace