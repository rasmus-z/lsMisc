#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cassert>
#include <stlsoft/smartptr/scoped_handle.hpp>

#include "gtest/gtest.h"

#include "../UrlEncode.h"
#include "../UTF16toUTF8.h"

using namespace std;
using namespace Ambiesoft;
TEST(UrlEncode, BasicWithChar)
{
	EXPECT_STREQ(UrlEncodeStd("aaa").c_str(), "aaa");
	EXPECT_STREQ(UrlEncodeStd("aaa a").c_str(), "aaa+a");
}
TEST(UrlEncode, BasicWithWchar)
{
	EXPECT_STREQ(wstring(UrlEncodeWstd(L"aaa")).c_str(), L"aaa");
	EXPECT_STREQ(wstring(UrlEncodeWstd(L"aaa a")).c_str(), L"aaa+a");
}
TEST(UrlEncode, Complex)
{
	{
		LPCSTR p = "https://social.msdn.microsoft.com/Search/en-US?query=isalnum&beta=0&ac=8";
		LPSTR penc = UrlEncode(p);
		stlsoft::scoped_handle<void*> mapenc(penc, free);

		LPBYTE pdec = UrlDecode(penc);
		stlsoft::scoped_handle<void*> mapdec(pdec, free);
		EXPECT_STREQ(p, (LPCSTR)pdec);
	}

	{
		LPCSTR p = "https://social.msdn.microsoft.com/Search/en-US?query=%e3%81%82%e3%81%b0%e3%81%b0%e3%81%b0%ef%bd%82%e3%81%98%e3%81%88%e3%81%88%ef%bd%97%e3%82%8c%ef%bd%97&beta=0&ac=8";
		LPSTR penc = UrlEncode(p);
		stlsoft::scoped_handle<void*> mapenc(penc, free);
		
		LPBYTE pdec = UrlDecode(penc);
		stlsoft::scoped_handle<void*> mapdec(pdec, free);
		EXPECT_STREQ(p, (LPCSTR)pdec);

		LPCWSTR wp = L"https://social.msdn.microsoft.com/Search/en-US?query=%e3%81%82%e3%81%b0%e3%81%b0%e3%81%b0%ef%bd%82%e3%81%98%e3%81%88%e3%81%88%ef%bd%97%e3%82%8c%ef%bd%97&beta=0&ac=8";
		wstring w1 = UrlEncodeWstd(wp);
		wstring w2 = UrlDecodeWstd(w1);
		EXPECT_EQ(wp, w2);
	}

	{
		//LPCSTR p = "あいうえお";
		//LPSTR penc = UrlEncode((BYTE*)p);
		//LPBYTE pdec = UrlDecode(penc);
		//assert(strcmp(p, (LPCSTR)pdec) == 0);

		// LPCWSTR wp = L"あいうえお";
		// "あいうえお"
		// UTF16-LE
		wchar_t wp[] = {0x3042,0x3044,0x3046,0x3048,0x304A,0x00};
		std::wstring wenc = UrlEncodeWstd(wp);
		std::wstring wdec = UrlDecodeWstd(wenc.c_str());
		EXPECT_STREQ(wp, wdec.c_str());
	}

}