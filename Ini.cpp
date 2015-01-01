#include <windows.h>
#include <tchar.h>
#include "Ini.h"

Ini::Ini(void)
{
	TCHAR szT[MAX_PATH];
	GetModuleFileName(NULL, szT, _countof(szT));

	*_tcsrchr(szT, _T('.'))=0;
	lstrcat(szT, _T(".ini"));

	f_ = _tcsdup(szT);
}

Ini::~Ini(void)
{
	free((void*)f_);
	f_ = NULL;
}

