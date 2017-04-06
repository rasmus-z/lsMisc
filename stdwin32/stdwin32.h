#pragma warning(disable:4786)
#pragma once

#include <vector>
#include <string>
#include <windows.h>
#include "../tstring.h"

namespace stdwin32 {

	std::string stdGetModuleFileNameA(HINSTANCE hInst=NULL);
	std::wstring stdGetModuleFileNameW(HINSTANCE hInst=NULL);
#ifdef UNICODE
	#define stdGetModuleFileName stdGetModuleFileNameW
#else
	#define stdGetModuleFileName stdGetModuleFileNameA
#endif




	BOOL stdIsFullPath(LPCWSTR pD);
	std::wstring stdCombinePath(LPCWSTR pD1, LPCWSTR pD2);
	std::wstring stdCombinePath(const std::wstring& d1, const std::wstring& d2);
	
	std::wstring stdGetParentDirectory(const std::wstring& path, bool bAddBackslach=false);
	std::wstring stdGetParentDirectory(LPCWSTR pPath, bool bAddBackslach=false);
	
	std::wstring stdGetFileName(const std::wstring& full);
	std::wstring stdGetFileName(LPCWSTR pFull);

	std::vector<std::wstring> stdSplitSCedPath(LPCWSTR pPath);
	


#ifndef __cplusplus_cli
	std::string string_formatA(const std::string fmt, ...);
	std::wstring string_formatW(const std::wstring fmt, ...);
#ifdef UNICODE
	#define string_format string_formatW
#else
	#define string_format string_formatA
#endif
#endif // __cplusplus_cli


	std::string trimA(const std::string& str,
					 const std::string& whitespace = " \t\r\n");
	std::wstring trimW(const std::wstring& str,
					 const std::wstring& whitespace = L" \t\r\n");
#ifdef UNICODE
	#define trim trimW
#else
	#define trim trimA
#endif



	bool isTdigit(const tstring& str);

	std::wstring stdGetCurrentDirectory();



	bool hasEndingA (std::string const &fullString, std::string const &ending);
	bool hasEndingW (std::wstring const &fullString, std::wstring const &ending);
#ifdef UNICODE
	#define hasEnding hasEndingW
#else
	#define hasEnding hasEndingA
#endif


	bool hasEndingIA (std::string const &fullString, std::string const &ending);
	bool hasEndingIW (std::wstring const &fullString, std::wstring const &ending);
#ifdef UNICODE
	#define hasEndingI hasEndingIW
#else
	#define hasEndingI hasEndingIA
#endif



	std::string stdItoA(int i);
	std::wstring stdItoW(int i);
#ifdef UNICODE
	#define stdItoT stdItoW
#else
	#define stdItoT stdItoA
#endif



	std::string stdItoA64(__int64 i);
	std::wstring stdItoW64(__int64 i);
#ifdef UNICODE
	#define stdItoT64 stdItoW64
#else
	#define stdItoT64 stdItoA
#endif


	std::string stdExpandEnvironmentStringsA(LPCSTR pStr);
	std::wstring stdExpandEnvironmentStringsW(LPCWSTR pStr);
#ifdef UNICODE
	#define stdExpandEnvironmentStrings stdExpandEnvironmentStringsW
#else
	#define stdExpandEnvironmentStrings stdExpandEnvironmentStringsA
#endif



	std::wstring stdGetEnvironmentVariableW(LPCWSTR pStr);
#ifdef UNICODE
	#define stdGetEnvironmentVariable stdGetEnvironmentVariableW
#else
	#define stdGetEnvironmentVariable stdGetEnvironmentVariableA
#endif



	bool stdGetUnittedSizeW(LPCWSTR pStr, int& nSign, __int64& lResult, int* pUnit=NULL);
#ifdef UNICODE
	#define stdGetUnittedSize stdGetUnittedSizeW
#else
	#define stdGetUnittedSize stdGetUnittedSizeA
#endif



	std::wstring stdSplitCommandLineW(int argc, int startargc, LPCWSTR* argv);
#ifdef UNICODE
#define stdSplitCommandLine stdSplitCommandLineW
#else
#define stdSplitCommandLine stdSplitCommandLineA
#endif


	std::wstring stdAddBackSlash(const std::wstring& d);
}

