#pragma once

BOOL WritePrivateProfileWString(const TCHAR *lpszSection, const TCHAR *lpszKey, const TCHAR *lpszIn, const TCHAR *lpszFile);
BOOL GetPrivateProfileWString(const TCHAR *lpszSection, const TCHAR *lpszKey, const TCHAR *lpszDefault, tstring& tout, const TCHAR *lpszFile);
