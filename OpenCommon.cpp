#include "stdafx.h"
#include "OpenCommon.h"

BOOL OpenCommon(HWND hWnd, LPCTSTR pApp, LPCTSTR pCommand, LPCTSTR pDirectory)
{
	SHELLEXECUTEINFO sei = {0};
	sei.cbSize = sizeof(sei);
	sei.fMask = 0;
	sei.hwnd = hWnd;
	sei.lpFile = pApp;
	sei.lpParameters = pCommand;
	sei.lpDirectory = pDirectory;
	sei.nShow = SW_SHOW;

	return ShellExecuteEx(&sei);
}