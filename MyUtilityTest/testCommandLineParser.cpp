#include "stdafx.h"
#include "../CommandLineParser.h"

void testCommandLine()
{
	Ambiesoft::CCommandLineParser clp;
	clp.Parse(__argc, __targv);
}