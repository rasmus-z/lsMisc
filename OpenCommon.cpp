#include "stdafx.h"

#ifndef _INC_SHELLAPI
#pragma message("including shllapi")
#include <Shellapi.h>
#endif

#ifndef _INC_SHLWAPI
#pragma message("including shlwlapi")
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#endif

#ifndef _STRING_
#pragma message("including string")
#include <string>
#endif


#include "OpenCommon.h"

namespace Ambiesoft {

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
		SHELLEXECUTEINFOA sei = { 0 };
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
} // namespace ambiesoft