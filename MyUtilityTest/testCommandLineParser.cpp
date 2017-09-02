#include "stdafx.h"
#include <tchar.h>

#include "../CommandLineParser.h"

using namespace Ambiesoft;

void testCommandLine()
{
	CCommandLineParser clp;

	COption opA(L"-a");
	clp.AddOption(&opA);

	clp.Parse(__argc, __targv);

	if(opA.hadOption())
	{
	}
}