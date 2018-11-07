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

TEST(CommandLineString, subStringRemove)
{
	LPWSTR pC1;

	{
		pC1 = L"aaa.exe \"aaa bbb\" aa fff feee 12345";
		CCommandLineString cls(pC1);
		cls.remove(2, 3);  // remove 3 items from index 2
		EXPECT_STREQ(L"aaa.exe \"aaa bbb\" 12345", cls.toString().c_str());
	}
	{
		pC1 = L"aaa.exe \"aaa bbb\" aa fff feee '123 45'";
		CCommandLineString cls(pC1);
		cls.remove(2, 3);  // remove 3 items from index 2
		EXPECT_STREQ(L"aaa.exe \"aaa bbb\" '123 45'", cls.toString().c_str());
	}
}

TEST(CommandLineString, ArgcArgv)
{
	LPWSTR pC1;
	pC1 = L"aaa.exe \"aaa bbb\" aa fff feee aaa\"bbb\" ";

	{
		CCommandLineString cms1(pC1);
		CCommandLineString cms2(pC1);
		EXPECT_EQ(cms1, cms2);
	}
	{
		CCommandLineString cms1(pC1);

		int nArg;
		LPWSTR* pArgv = CommandLineToArgvW(pC1, &nArg);
		CCommandLineString cms2(nArg, pArgv);

		EXPECT_NE(cms1, cms2);
		EXPECT_TRUE(cms1.SyntaxEqual(cms2));
	}

	{
		wchar_t* argv[] = {
			L"exe.exe",
			L"-h",
			L"-b",
			L"-path",
			L"c:\\aaa\ttt bbb\aa",
			L"file1.txt",
			L"file2.txt",
			L"file3.txt",
			L"space file.txt",
			NULL
		};

		int argc = _countof(argv) - 1;
		CCommandLineString cms(argc, argv);
		EXPECT_EQ(cms.getCount(), argc);
		for (int i = 0; i < argc; ++i)
		{
			EXPECT_STREQ(cms.getArg(i).c_str(), argv[i]);
		}
	}
}

TEST(CommandLineString, DQinsideDQ)
{
	LPWSTR pC1;
	pC1 = L"aaa.exe \"-DCR_CLANG_REVISION=\\\"336424-1\\\"\"";
	EXPECT_TRUE(isSameResult(pC1));

	pC1 = L"aaa.exe \"-DCR_CLANG_REVISION=\\\"336424-1\\\"";
	EXPECT_TRUE(isSameResult(pC1));

	pC1 = L"aaa.exe \"-DCR_CLANG_REVISION=\\\"336424-1\\";
	EXPECT_TRUE(isSameResult(pC1));
}