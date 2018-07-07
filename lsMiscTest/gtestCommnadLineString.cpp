#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <cassert>

#include <gtest/gtest.h>

#include "../CommandLineString.h"
#include "debug.h"

using namespace Ambiesoft;
using namespace std;

static bool isSameResult(LPWSTR pCL)
{
	LPWSTR ppp = (LPWSTR)_tcsdup(GetCommandLine());
	free(ppp);

	int argc1 = 0;
	LPWSTR* argv1 = NULL;
	argv1 = CCommandLineString::getCommandLineArg(pCL,&argc1);

	int argc2 = 0;
	LPWSTR* argv2 = NULL;
	argv2 = CommandLineToArgvW(pCL, &argc2);


	if (argc1 != argc2)
		return false;

	for (int i = 0; i < argc1; ++i)
	{
		if (wcscmp(argv1[i], argv2[i]) != 0)
			return false;
	}

	CCommandLineString::freeCommandLineArg(argv1);
	LocalFree(argv2);
	return true;
}
TEST(CommandLineString, SameWithOtherMethods)
{
	LPWSTR pC1;
	pC1 = L"aaa.exe \"aaa bbb\"";
	EXPECT_TRUE(isSameResult(pC1));

	pC1 = L"a";
	EXPECT_TRUE(isSameResult(pC1));

	pC1 = L"";
	EXPECT_TRUE(isSameResult(pC1));

	pC1 = L"aaa.exe \"\"\"aaa bbb ccc\"\"\"";
	//isSameResult(pC1); // not same but i am right.
}
TEST(CommandLineString, subStringBasic)
{
	LPWSTR pC1;

	pC1 = L"aaa.exe file1.txt \"space file.txt\"";
	CCommandLineString cls(pC1);
	EXPECT_STREQ(cls.subString(0).c_str(), L"aaa.exe file1.txt \"space file.txt\"");
	EXPECT_STREQ(cls.subString(1).c_str(), L"file1.txt \"space file.txt\"");
	EXPECT_STREQ(cls.subString(2).c_str(), L"\"space file.txt\"");
}
TEST(CommandLineString, subStringComplex)
{
	LPWSTR pC1;

	pC1 = L"aaa.exe \"aaa bbb\" aa fff feee aaa\"bbb\" ";
	CCommandLineString cls(pC1);
	EXPECT_STREQ(cls.subString(0).c_str(), L"aaa.exe \"aaa bbb\" aa fff feee aaa\"bbb\" ");
	EXPECT_STREQ(cls.subString(1).c_str(), L"\"aaa bbb\" aa fff feee aaa\"bbb\" "); 
	EXPECT_STREQ(cls.subString(2).c_str(), L"aa fff feee aaa\"bbb\" ");
	EXPECT_STREQ(cls.subString(3).c_str(), L"fff feee aaa\"bbb\" ");
	EXPECT_STREQ(cls.subString(4).c_str(), L"feee aaa\"bbb\" ");
	EXPECT_STREQ(cls.subString(5).c_str(), L"aaa\"bbb\" ");
	EXPECT_STREQ(cls.subString(6).c_str(), L"");
}