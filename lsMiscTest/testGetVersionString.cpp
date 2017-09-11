#include "stdafx.h"
#include <tchar.h>
#include <iostream>

#include "../GetVersionString.h"

using namespace std;

void testGetVersionString()
{
	TCHAR buf[MAX_PATH];
	// Get file description for explorer.exe
	if(GetVersionString(_T("C:\\windows\\explorer.exe"), _T("FileDescription"), buf, MAX_PATH))
	{
		cout << buf << endl;
	}
}