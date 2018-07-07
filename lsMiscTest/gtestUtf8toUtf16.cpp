
#include "gtest/gtest.h"

#include "../UTF16toUTF8.h"

using namespace Ambiesoft;

TEST(UTF8TOUTF16, convert8to16)
{
	wchar_t* pW = UTF8toUTF16((LPBYTE)"aaa");
	EXPECT_TRUE(wcscmp(pW, L"aaa")==0);
}