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

#pragma once
#include <malloc.h>


inline static size_t GetConverToWCount(LPCSTR pIN)
{
	return MultiByteToWideChar(
		CP_ACP,
		0,
		pIN,
		-1,
		NULL,
		0);
}
inline static void ConvertToW(LPCSTR pIN, size_t inlen, LPWSTR* ppOut, size_t outlen)
{
	if (!pIN)
	{
		*ppOut = NULL;
		return;
	}

	if (!pIN[0])
	{
		*ppOut = L"";
		return;
	}

	MultiByteToWideChar(
		CP_ACP,
		0,
		pIN,
		(int)inlen,
		*ppOut,
		(int)outlen
		);
}

#define CREATEWSTRING(s)									\
	LPWSTR s##W;											\
	if (s == NULL)											\
	{														\
		s##W = NULL;										\
	}														\
	else if (s[0] == 0)										\
	{														\
		s##W = L"";											\
	}														\
	else													\
	{														\
		size_t s##Wcount = GetConverToWCount(s);			\
		s##W = (LPWSTR)_alloca(s##Wcount*sizeof(WCHAR));	\
		ConvertToW(s, -1, &s##W, s##Wcount);				\
	};
