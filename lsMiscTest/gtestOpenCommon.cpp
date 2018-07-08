#include "stdafx.h"
#include <string>

#include "gtest/gtest.h"

#include "../stdwin32/stdwin32.h"
#include "../OpenCommon.h"

using namespace std;
using namespace stdwin32;
using namespace Ambiesoft;

TEST(OpenCommon, ThisFolder)
{
	wstring thisexe = stdGetModuleFileName();
	EXPECT_EQ(OpenFolder(NULL, thisexe.c_str()), TRUE);
}