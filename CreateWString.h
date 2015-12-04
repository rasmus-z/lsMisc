#pragma once
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
		inlen,
		*ppOut,
		outlen
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
