#pragma once

namespace Ambiesoft {

	BOOL OpenCommonW(
		HWND hWnd,
		LPCWSTR pApp,
		LPCWSTR pCommand = NULL,
		LPCWSTR pDirectory = NULL,
		HANDLE* pPHandle = NULL,
		LPCWSTR pVerb = NULL);
	BOOL OpenCommonA(
		HWND hWnd,
		LPCSTR pApp,
		LPCSTR pCommand = NULL,
		LPCSTR pDirectory = NULL,
		HANDLE* pPHandle = NULL,
		LPCSTR pVerb = NULL);

	inline BOOL OpenCommon(
		HWND hWnd,
		LPCTSTR pApp,
		LPCTSTR pCommand = NULL,
		LPCTSTR pDirectory = NULL,
		HANDLE* pPHandle = NULL,
		LPCTSTR pVerb = NULL)
	{
#ifdef UNICODE
		return OpenCommonW(
#else
		return OpenCommonA(
#endif
			hWnd,
			pApp,
			pCommand,
			pDirectory,
			pPHandle,
			pVerb);
	}

	BOOL OpenFolderW(HWND h, LPCWSTR pFileOrFolder);
	BOOL OpenFolderA(HWND h, LPCSTR pFileOrFolder);
	inline BOOL OpenFolder(HWND h, LPCTSTR pFileOrFolder)
	{
#ifdef UNICODE
		return OpenFolderW(
#else
		return OpenFolderA(
#endif
			h,
			pFileOrFolder);
	}

} // namespace Ambiesoft