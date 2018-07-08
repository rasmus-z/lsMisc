#include "stdafx.h"

#include "gtest/gtest.h"

#include "../showBalloon.h"

using namespace Ambiesoft;



TEST(ShowBalloon, Show)
{
	EXPECT_TRUE(showballoon(
				 NULL,
				 L"tttitle",
				 L"teeeeeeeext",
				 NULL,
				 3000,
				 3332,
				 FALSE,
 				 1));
}