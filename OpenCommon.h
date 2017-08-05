#pragma once

BOOL OpenCommonW(HWND hWnd,
	LPCWSTR pApp,
	LPCWSTR pCommand = NULL,
	LPCWSTR pDirectory = NULL,
	HANDLE* pPHandle = NULL,
	LPCWSTR pVerb = NULL);
BOOL OpenCommonA(HWND hWnd,
	LPCSTR pApp,
	LPCSTR pCommand = NULL,
	LPCSTR pDirectory = NULL,
	HANDLE* pPHandle = NULL,
	LPCSTR pVerb = NULL);

#ifdef UNICODE
#define OpenCommon OpenCommonW
#else
#define OpenCommon OpenCommonA
#endif

void OpenFolderW(HWND h, LPCWSTR pFolder);
void OpenFolderA(HWND h, LPCSTR pFolder);

#ifdef UNICODE
#define OpenFolder OpenFolderW
#else
#define OpenFolder OpenFolderA
#endif
