#include "stdafx.h"
#include <string>
#include "FormatSizeof.h"

std::wstring FormatSizeof(double num)
{
	static LPCWSTR units[] = { L"", L"K", L"M", L"G", L"T", L"P", L"E", L"Z" };

	std::wstring ret;
	wchar_t szT[64];
	for (int i = 0; i < _countof(units); ++i)
	{
		LPCWSTR pUnit = units[i];
		if (abs(num) < 1024.0)
		{
			swprintf_s(szT, L"%3.1f%s", num, pUnit);
			ret = szT;
			return ret;
		}
		num /= 1024.0;
	}

	swprintf_s(szT, L"%.1f%s%s", num, L"Y");
	ret = szT;
	return ret;
}
std::wstring FormatSizeof(long long ll)
{
	return FormatSizeof((double)ll);
}