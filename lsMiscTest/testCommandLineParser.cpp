#include "stdafx.h"
#include <tchar.h>

#include "../CommandLineParser.h"

using namespace Ambiesoft;

void testCommandLine()
{
	bool isHelp = false;
	CCommandLineParser clp;
	clp.AddOption(L"-h", 0, &isHelp);
	
	wchar_t* argv[] = {
		L"exe.exe",
		L"-h",
		NULL
	};
	clp.Parse(2, argv);

	assert(isHelp);
}