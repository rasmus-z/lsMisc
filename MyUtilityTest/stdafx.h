#include <Windows.h>
#include <Tlhelp32.h>
#include <Psapi.h>
#include <Shlwapi.h>

#include <tchar.h>
#include <cassert>
#include <string>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>

#define DVERIFY(a) assert(a)

inline void mytrace(LPCWSTR p)
{
	OutputDebugString(p);
	OutputDebugString(L"\r\n");
}
inline void mytrace(const std::wstring& ss)
{
	OutputDebugString(ss.c_str());
	OutputDebugString(L"\r\n");
}