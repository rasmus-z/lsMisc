#include "stdafx.h"
#include <tchar.h>
#include <string>
#include <gtest/gtest.h>

#include "../ChangeFilenamable.h"

using namespace std;

TEST(ChangeFilenamable, Basic)
{
	LPCTSTR p = _T("開発機構特別資料");
	wstring ret = ChangeFilenamable(p);
	EXPECT_STREQ(p, ret.c_str());

	p = _T("aaa\\bbb");
	ret = ChangeFilenamable(p);
	EXPECT_STREQ(ret.c_str(), _T("aaabbb"));
}