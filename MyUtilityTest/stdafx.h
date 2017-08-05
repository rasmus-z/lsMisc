#include <Windows.h>
#include <tchar.h>
#include <cassert>
#include <string>

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