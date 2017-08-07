#include "stdafx.h"
#include <Shellapi.h>
#include <Shlwapi.h>
#include <string>

#pragma comment(lib,"shlwapi.lib")

#include "OpenCommon.h"

BOOL OpenCommonW(HWND hWnd,
	LPCWSTR pApp,
	LPCWSTR pCommand,
	LPCWSTR pDirectory,
	HANDLE* pPHandle,
	LPCWSTR pVerb)
{
	SHELLEXECUTEINFOW sei = { 0 };
	sei.cbSize = sizeof(sei);
	sei.fMask = 0;
	if (pPHandle)
	{
		sei.fMask |= SEE_MASK_NOCLOSEPROCESS;
	}

	sei.fMask |= SEE_MASK_UNICODE;

	sei.hwnd = hWnd;
	sei.lpFile = pApp;
	sei.lpParameters = pCommand;
	sei.lpDirectory = pDirectory;
	sei.nShow = SW_SHOW;
	sei.lpVerb = pVerb;

	BOOL ret = ShellExecuteExW(&sei);
	if (ret && pPHandle)
	{
		*pPHandle = sei.hProcess;
	}

	return ret;
}

BOOL OpenCommonA(HWND hWnd, 
				LPCSTR pApp, 
				LPCSTR pCommand,
				LPCSTR pDirectory,
				HANDLE* pPHandle,
				LPCSTR pVerb)
{
	SHELLEXECUTEINFOA sei = {0};
	sei.cbSize = sizeof(sei);
	sei.fMask = 0;
	if (pPHandle)
	{
		sei.fMask |= SEE_MASK_NOCLOSEPROCESS;
	}

	sei.hwnd = hWnd;
	sei.lpFile = pApp;
	sei.lpParameters = pCommand;
	sei.lpDirectory = pDirectory;
	sei.nShow = SW_SHOW;
	sei.lpVerb = pVerb;

	BOOL ret = ShellExecuteExA(&sei);
	if (ret && pPHandle)
	{
		*pPHandle = sei.hProcess;
	}

	return ret;
}

BOOL OpenFolderA(HWND h, LPCSTR pFileOrFolder)
{
	if (!PathFileExistsA(pFileOrFolder) && !PathIsDirectoryA(pFileOrFolder))
		return FALSE;

	std::string arg = "/select,\"";
	arg += pFileOrFolder;
	arg += "\",/n";
	ShellExecuteA(h,
		NULL,
		"explorer.exe",
		arg.c_str(),
		NULL,
		SW_SHOW);

	return TRUE;
}
BOOL OpenFolderW(HWND h, LPCWSTR pFileOrFolder)
{
	if (!PathFileExistsW(pFileOrFolder) && !PathIsDirectoryW(pFileOrFolder))
		return FALSE;

	std::wstring arg = L"/select,\"";
	arg += pFileOrFolder;
	arg += L"\",/n";
	ShellExecuteW(h,
		NULL,
		L"explorer.exe",
		arg.c_str(),
		NULL,
		SW_SHOW);

	return TRUE;
}