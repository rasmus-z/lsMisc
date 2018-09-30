//#include "stdafx.h"

#include <cassert>
#include <string.h>
#include <string>

#include "gtest/gtest.h"

#include "../stdosd/PathUtil.h"

// #include "debug.h"

using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;
using namespace std;

TEST(PathUtil, IsRoot)
{
    EXPECT_TRUE(!IsPathRoot(L"C:"));
    EXPECT_TRUE(!IsPathRoot(L"C:\\"));
	EXPECT_TRUE(!IsPathRoot(L"C:\\a"));
    EXPECT_TRUE(!IsPathRoot(L"\\\\"));
    EXPECT_TRUE(!IsPathRoot(L"\\\\thexp"));
	EXPECT_TRUE(!IsPathRoot(L"\\\\thexp\\a"));
	EXPECT_TRUE(!IsPathRoot(L"\\\\thexp\\a\\b"));
	EXPECT_TRUE(!IsPathRoot(L""));
	EXPECT_TRUE(!IsPathRoot(L".."));
    EXPECT_TRUE(!IsPathRoot(L"../../../../../../../../../../../../../../../../../../../../../"));

    EXPECT_TRUE(!IsPathRoot(L"/afwefwea/fjweo/fwejo"));

    EXPECT_TRUE(!IsPathRoot(L"//"));


    EXPECT_TRUE(IsPathRoot(L"/"));
    EXPECT_TRUE(IsPathRoot(L"../../.."));
}

TEST(PathUtil, IsChild)
{
    wstring p1, p2;
    wstring duppath;

    p1 = L"/a/b/c";
    p2 = L"/a/b/d";
    EXPECT_TRUE(!IsPathChildIncluded(p1.c_str(), p2.c_str()));

    p1 = L"/a/b/c/../..";
    p2 = L"/a/b/d";
    EXPECT_TRUE(IsPathChildIncluded(p1.c_str(), p2.c_str(), &duppath));
    EXPECT_TRUE(wcscmp(duppath.c_str(), L"c:\\a")==0);

	p1 = L"\\\\thexp\\share\\";
	p2 = L"\\\\THEXP\\share\\b";
    EXPECT_TRUE(IsPathChildIncluded(p1.c_str(), p2.c_str(), &duppath));
    EXPECT_TRUE(wcscmp(duppath.c_str(), L"\\\\thexp\\share") == 0);

	p1 = L"\\\\thexp\\share\\xxx";
	p2 = L"\\\\THEXP\\share\\b";
    EXPECT_TRUE(!IsPathChildIncluded(p1.c_str(), p2.c_str(), &duppath));

	p1 = L"\\\\thexp\\share\\aaa";
	p2 = L"\\\\THEXP\\share\\aaa\\vvvsdf\\fwefew";
    EXPECT_TRUE(IsPathChildIncluded(p1.c_str(), p2.c_str(), &duppath));
    EXPECT_TRUE(wcscmp(duppath.c_str(), L"\\\\thexp\\share\\aaa") == 0);


    p1 = L"/a/b/x/f";
    p2 = L"/a/b/y";
    EXPECT_TRUE(!IsPathChildIncluded(p1.c_str(), p2.c_str(), &duppath));

    p1 = L"/t";
    p2 = L"/";
    EXPECT_TRUE(IsPathChildIncluded(p1.c_str(), p2.c_str(), &duppath));

	p1 = L"/a/b/c/d";
	p2 = L"/a/b/c/d/e";
    EXPECT_TRUE(IsPathChildIncluded(p1.c_str(), p2.c_str(), &duppath));
}
