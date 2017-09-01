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
#endif  // __cplusplus_cli
