#include "stdafx.h"

#include <vcclr.h>
#include <string>

#include "clrString.h"

#ifdef __cplusplus_cli

using namespace System;
using namespace std;

String^ toCLR(const wstring& s)
{
	return gcnew String(s.c_str());
}

wstring toWstring(String^ s)
{
	pin_ptr<const wchar_t> pS = PtrToStringChars(s);
	return pS;
}



String^ doubleQuote(String^ s)
{
	if (String::IsNullOrEmpty(s))
		return s;
	if (s[0] == L'"')
		return s;

	bool needquote = false;
	for each(Char c in s)
	{
		if (Char::IsWhiteSpace(c))
		{
			needquote = true;
			break;
		}
	}

	return needquote ? L"\"" + s + L"\"" : s;
}






#endif  // __cplusplus_cli
