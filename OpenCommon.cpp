#include "stdafx.h"
#include <Shellapi.h>
#include "OpenCommon.h"

BOOL OpenCommon(HWND hWnd, 
				LPCTSTR pApp, 
				LPCTSTR pCommand, 
				LPCTSTR pDirectory, 
				HANDLE* pPHandle,
				LPCTSTR pVerb)
{
	SHELLEXECUTEINFO sei = {0};
	sei.cbSize = sizeof(sei);
	sei.fMask = 0;
	if (pPHandle)
	{
		sei.fMask |= SEE_MASK_NOCLOSEPROCESS;
	}
#ifdef UNICODE
	sei.fMask |= SEE_MASK_UNICODE;
#endif

	sei.hwnd = hWnd;
	sei.lpFile = pApp;
	sei.lpParameters = pCommand;
	sei.lpDirectory = pDirectory;
	sei.nShow = SW_SHOW;
	sei.lpVerb = pVerb;

	BOOL ret = ShellExecuteEx(&sei);
	if (ret && pPHandle)
	{
		*pPHandle = sei.hProcess;
	}

	return ret;
}

