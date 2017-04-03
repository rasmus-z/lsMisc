#pragma once
BOOL ReplaceFileHelper(
	LPCTSTR lpReplacedFileName,
	LPCTSTR lpReplacementFileName,
	LPCTSTR lpBackupFileName,
	DWORD   dwReplaceFlags,
	DWORD* pdwLastError=NULL);