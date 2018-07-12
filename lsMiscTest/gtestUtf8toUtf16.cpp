#include "StdAfx.h"
#include <memory>
#include "gtest/gtest.h"

#include "../UTF16toUTF8.h"

using namespace Ambiesoft;

static void MyFree(void* p)
{
	free(p);
}
TEST(UTF8TOUTF16, convert8to16)
{
	std::unique_ptr<char[]> p(new char[3]);
	std::unique_ptr<wchar_t[], void(*)(void*)> pW( UTF8toUTF16((LPBYTE)"aaa"), MyFree);
	EXPECT_TRUE(wcscmp(pW.get(), L"aaa")==0);
}