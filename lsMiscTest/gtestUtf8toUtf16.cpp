#include "StdAfx.h"
#include <memory>
#include "gtest/gtest.h"

#include "../UTF16toUTF8.h"

using namespace Ambiesoft;
using namespace std;

static void MyFree(void* p)
{
	free(p);
}
TEST(UTF8TOUTF16, convert8to16)
{
	{
		std::unique_ptr<char[]> p(new char[3]);
		std::unique_ptr<wchar_t, void(*)(void*)> pW(UTF8toUTF16((LPBYTE)"aaa"), MyFree);
		EXPECT_TRUE(wcscmp(pW.get(), L"aaa") == 0);
	}
	{
		int outlen;
		std::unique_ptr<wchar_t, void(*)(void*)> pW(UTF8toUTF16((LPBYTE)"aaa", 3, &outlen), MyFree);
		EXPECT_TRUE(wcscmp(pW.get(), L"aaa") == 0);
		EXPECT_EQ(outlen, 3);
	}
	{
		int outlen;
		std::unique_ptr<wchar_t, void(*)(void*)> pW(UTF8toUTF16((LPBYTE)"abcdef", 2, &outlen), MyFree);
		EXPECT_TRUE(wcscmp(pW.get(), L"ab") == 0);
		EXPECT_EQ(outlen, 2);
	}

	{
		string s = "a";
		std::unique_ptr<wchar_t, void(*)(void*)> pW(UTF8toUTF16((LPBYTE)s.c_str()), MyFree);
		EXPECT_STREQ(pW.get(), L"a");
	}

	{
		int outlen;
		LPCWSTR p = MultiBytetoUTF16(CP_ACP, "", 0, &outlen);
		EXPECT_STREQ(p, L"");
		EXPECT_EQ(outlen, 0);
		free((void*)p);
	}

	{
		int outlen;
		LPCWSTR p = MultiBytetoUTF16(CP_ACP, "aaa", 3, &outlen);
		EXPECT_STREQ(p, L"aaa");
		EXPECT_EQ(outlen, 3);
		free((void*)p);
	}

	{
		string s = "aaa";
		EXPECT_EQ(toStdWstringFromUtf8(s), wstring(L"aaa"));
	}
}

TEST(UTF8TOUTF16, convert16to8)
{
	{
		std::unique_ptr<char, void(*)(void*)> p((char*)UTF16toUTF8(L"aaa"), MyFree);
		EXPECT_STREQ(p.get(), "aaa");
	}
	{
		int outlen;
		std::unique_ptr<char, void(*)(void*)> p((char*)UTF16toUTF8(L"aaa",3,&outlen), MyFree);
		EXPECT_STREQ(p.get(), "aaa");
		EXPECT_EQ(outlen, 3);
	}

	{
		int outlen;
		std::unique_ptr<char, void(*)(void*)> p((char*)UTF16toMultiByte(CP_ACP, L"aaa", 3, &outlen), MyFree);
		EXPECT_STREQ(p.get(), "aaa");
		EXPECT_EQ(outlen, 3);
	}
	{
		int outlen;
		std::unique_ptr<char, void(*)(void*)> p((char*)UTF16toMultiByte(CP_ACP, L"", 0, &outlen), MyFree);
		EXPECT_STREQ(p.get(), "");
		EXPECT_EQ(outlen, 0);
	}
	{
		int outlen;
		std::unique_ptr<char, void(*)(void*)> p((char*)UTF16toMultiByte(CP_ACP, nullptr, 0, &outlen), MyFree);
		EXPECT_STREQ(p.get(), nullptr);
		EXPECT_EQ(outlen, 0);
	}
	{
		wstring s = L"sss";
		EXPECT_EQ(toStdString(s), string("sss"));
	}
}