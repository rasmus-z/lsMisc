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
		std::unique_ptr<wchar_t> pW(UTF8toUTF16_new("aaa"));
		EXPECT_TRUE(wcscmp(pW.get(), L"aaa") == 0);
	}
	{
		int outlen;
		std::unique_ptr<wchar_t> pW(UTF8toUTF16_new("aaa", 3, &outlen));
		EXPECT_TRUE(wcscmp(pW.get(), L"aaa") == 0);
		EXPECT_EQ(outlen, 3);
	}
	{
		int outlen;
		std::unique_ptr<wchar_t> pW(UTF8toUTF16_new("abcdef", 2, &outlen));
		EXPECT_TRUE(wcscmp(pW.get(), L"ab") == 0);
		EXPECT_EQ(outlen, 2);
	}

	{
		string s = "a";
		std::unique_ptr<wchar_t> pW(UTF8toUTF16_new(s.c_str()));
		EXPECT_STREQ(pW.get(), L"a");
	}

	{
		int outlen;
		LPCWSTR p = MultiBytetoUTF16_new(CP_ACP, "", 0, &outlen);
		EXPECT_STREQ(p, L"");
		EXPECT_EQ(outlen, 0);
		delete p;
	}

	{
		int outlen;
		LPCWSTR p = MultiBytetoUTF16_new(CP_ACP, "aaa", 3, &outlen);
		EXPECT_STREQ(p, L"aaa");
		EXPECT_EQ(outlen, 3);
		delete p;
	}

	{
		string s = "aaa";
		EXPECT_EQ(toStdWstringFromUtf8(s), wstring(L"aaa"));
	}
}

TEST(UTF8TOUTF16, convert16to8)
{
	{
		std::unique_ptr<char> p(UTF16toUTF8_new(L"aaa"));
		EXPECT_STREQ(p.get(), "aaa");
	}
	{
		int outlen;
		std::unique_ptr<char> p(UTF16toUTF8_new(L"aaa", 3, &outlen));
		EXPECT_STREQ(p.get(), "aaa");
		EXPECT_EQ(outlen, 3);
	}

	{
		int outlen;
		std::unique_ptr<char> p(UTF16toMultiByte_new(CP_ACP, L"aaa", 3, &outlen));
		EXPECT_STREQ(p.get(), "aaa");
		EXPECT_EQ(outlen, 3);
	}
	{
		int outlen;
		std::unique_ptr<char> p(UTF16toMultiByte_new(CP_ACP, L"", 0, &outlen));
		EXPECT_STREQ(p.get(), "");
		EXPECT_EQ(outlen, 0);
	}
	{
		int outlen;
		std::unique_ptr<char> p(UTF16toMultiByte_new(CP_ACP, nullptr, 0, &outlen));
		EXPECT_STREQ(p.get(), nullptr);
		EXPECT_EQ(outlen, 0);
	}
	{
		wstring s = L"sss";
		EXPECT_EQ(toStdUtf8String(s), string("sss"));
	}
}