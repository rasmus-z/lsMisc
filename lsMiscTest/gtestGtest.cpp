#include "stdafx.h"
#include <gtest/gtest.h>

using namespace std;

void MyExit(bool death,int exitCode)
{
	if (death)
	{
		cerr << "this is my death test" << endl;
		exit(exitCode);
	}
	cerr << "I'm Alive!!" << endl;
}
bool MyExited(int exitCode, int exitedCode)
{
	return exitCode == exitedCode;
}
TEST(DISABLED_GTestTest, DeathTest)
{
	int exitCode = 0;
	EXPECT_EXIT(MyExit(true, 1), ::testing::ExitedWithCode(1), "this is my death test");
	EXPECT_EXIT(MyExit(true, 54321), std::bind(MyExited, 54321, std::placeholders::_1), "this is my death test");
	EXPECT_DEATH(MyExit(true, 1), "this is my death test");

    // failed test
	EXPECT_EXIT(MyExit(false, 1), ::testing::ExitedWithCode(1), "this is my death test");
	EXPECT_EXIT(MyExit(true, 12345), std::bind(MyExited, 54321, std::placeholders::_1), "this is my death test");
	EXPECT_DEATH(MyExit(true, 1), "this is NOT my death test");
}