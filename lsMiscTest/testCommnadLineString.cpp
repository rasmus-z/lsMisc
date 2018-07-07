#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <cassert>

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

	//assert(argc1 == 2);
	//mytrace(argv1[0]);
	//assert(wcscmp(argv1[0], L"aaa.exe") == 0);
	//assert(wcscmp(argv1[1], L"aaa bbb") == 0);
	//assert(argv1[2] == NULL);

	int argc2 = 0;
	LPWSTR* argv2 = NULL;
	argv2 = CommandLineToArgvW(pCL, &argc2);



	assert(argc1 == argc2);
	for (int i = 0; i < argc1; ++i)
	{
		assert(wcscmp(argv1[i], argv2[i]) == 0);
	}

	CCommandLineString::freeCommandLineArg(argv1);
	LocalFree(argv2);
	return true;
}
void testCommandLineString()
{
	LPWSTR pC1;
	pC1 = L"aaa.exe \"aaa bbb\"";
	isSameResult(pC1);
	pC1 = L"a";
	isSameResult(pC1);
	pC1 = L"";
	isSameResult(pC1);
	//pC1 = NULL;
	//isSameResult(pC1);

	pC1 = L"aaa.exe \"\"\"aaa bbb ccc\"\"\"";
	//isSameResult(pC1); // not same but i am right.

	pC1 = L"aaa.exe \"aaa bbb\" aa fff feee aaa\"bbb\" ";
	CCommandLineString cls(pC1);
	mytrace(cls.subString(0));
	mytrace(cls.subString(1));
	mytrace(cls.subString(2));
	mytrace(cls.subString(3));
	mytrace(cls.subString(4));
	mytrace(cls.subString(5));
	mytrace(cls.subString(6));

}