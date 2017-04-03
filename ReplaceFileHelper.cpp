#include "stdafx.h"
#include <windows.h>
#include <tchar.h>

#include "ReplaceFileHelper.h"

typedef BOOL (WINAPI *fnReplaceFile)(
									 LPCTSTR lpReplacedFileName,
									 LPCTSTR lpReplacementFileName,
									 LPCTSTR lpBackupFileName,
									 DWORD   dwReplaceFlags,
									 LPVOID  lpExclude,
									 LPVOID  lpReserved
									 );

#ifdef UNICODE
#define AW(name) name "W"
#else
#define AW(name) name "A"
#endif

BOOL ReplaceFileHelper(
					   LPCTSTR lpReplacedFileName,
					   LPCTSTR lpReplacementFileName,
					   LPCTSTR lpBackupFileName,
					   DWORD   dwReplaceFlags,
					   DWORD* pdwLastError)
{
	BOOL bRet = FALSE;
	BOOL bProcessed = FALSE;
	BOOL bReplacedExists = (GetFileAttributes(lpReplacedFileName) != 0xFFFFFFFF);
	if(bReplacedExists)
	{
		HMODULE h = LoadLibrary(_T("Kernel32.dll"));
		if(h)
		{
			LPCSTR pFuncName = AW("ReplaceFile");
			fnReplaceFile func = (fnReplaceFile)GetProcAddress(h,pFuncName);
			if(func)
			{
				bRet = func(
					lpReplacedFileName,
					lpReplacementFileName,
					lpBackupFileName,
					dwReplaceFlags,
					NULL,NULL);
				if(!bRet && pdwLastError)
					*pdwLastError = GetLastError();
				bProcessed = TRUE;
			}
			FreeLibrary(h);
		}
	}

	if(bProcessed)
		return bRet;

	if(bReplacedExists && lpBackupFileName && *lpBackupFileName)
	{
		DeleteFile(lpBackupFileName);
		if(!MoveFile(lpReplacedFileName, lpBackupFileName))
		{
			if(pdwLastError)
				*pdwLastError = GetLastError();
			return FALSE;
		}
	}

	if(!MoveFile(lpReplacementFileName, lpReplacedFileName))
	{
		if(pdwLastError)
			*pdwLastError = GetLastError();
		return FALSE;
	}

	if(pdwLastError)
		*pdwLastError = NO_ERROR;
	return TRUE;
}
