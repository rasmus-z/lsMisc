#include "stdafx.h"
#include <tchar.h>
#include <iostream>

#include "../CommandLineParser.h"

using namespace Ambiesoft;
using namespace std;

template <class InputIterator>
bool outvalue(InputIterator first, InputIterator last)
{
	while (first != last) {
		wcout << *first << endl;
		++first;
	}
	return false;
}

#define ACTULAPATH L"c:\\aaa\ttt bbb\aa"
void testCommandLine()
{
	vector<wstring> vs = { L"-a", L"-b", L"-c" };
	outvalue(vs.begin(), vs.end());

	wchar_t* opXYZ[] = {
		L"-xyz",
		L"-bbb",
	};

	bool isHelp = false;
	bool isABC;
	bool isXYZ;
	wstring path;

	CCommandLineParser clp;
	clp.AddOption(L"-h", L"/?", 0, &isHelp);
	clp.AddOption(vs.begin(), vs.end(), 0, &isABC);
	clp.AddOption(opXYZ, opXYZ + _countof(opXYZ), 0, &isXYZ);
	clp.AddOption(L"-path", 1, &path);

	wchar_t* argv[] = {
		L"exe.exe",
		L"-h",
		L"-b",
		L"-path",
		ACTULAPATH,
		NULL
	};
	clp.Parse(_countof(argv)-1, argv);

	assert(isHelp);
	assert(isABC);
	assert(path == ACTULAPATH);
}