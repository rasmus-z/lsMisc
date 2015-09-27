#pragma warning(disable:4786)

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
	std::wstring stdGetParentDirectory(LPCWSTR pPath);
	std::vector<std::wstring> stdSplitSCedPath(LPCWSTR pPath);
	



	std::string string_formatA(const std::string fmt, ...);
	std::wstring string_formatW(const std::wstring fmt, ...);
#ifdef UNICODE
	#define string_format string_formatW
#else
	#define string_format string_formatA
#endif



	std::string trimA(const std::string& str,
					 const std::string& whitespace = " \t");
	std::wstring trimW(const std::wstring& str,
					 const std::wstring& whitespace = L" \t");
#ifdef UNICODE
	#define trim trimW
#else
	#define trim trimA
#endif


	bool isTdigit(const tstring& str);
}