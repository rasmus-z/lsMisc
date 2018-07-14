#include "stdafx.h"

#include "../stdosd/stdosd.h"


using namespace Ambiesoft::stdosd;
using namespace std;



TEST(stdosd, stdIsDigit)
{
	// "１２３"
	// UTF16-LE
	wchar_t zenNum[] = { 0xFF11,0xFF12,0xFF13,0x00 };
	EXPECT_TRUE(stdIsAsciiDigit(L"123"));
	EXPECT_TRUE(stdIsAsciiDigit("0993"));
	EXPECT_TRUE(stdIsAsciiDigit(wstring(L"123")));
	EXPECT_TRUE(stdIsAsciiDigit(string("09")));
	EXPECT_FALSE(stdIsAsciiDigit(zenNum));
	EXPECT_FALSE(stdIsAsciiDigit(wstring(zenNum)));

	EXPECT_TRUE(stdIsTdigit(L"123"));
	EXPECT_TRUE(stdIsTdigit("0993"));
	EXPECT_TRUE(stdIsTdigit(wstring(L"123")));
	EXPECT_TRUE(stdIsTdigit(string("09")));
	EXPECT_TRUE(stdIsTdigit(zenNum));
	EXPECT_TRUE(stdIsTdigit(wstring(zenNum)));
}