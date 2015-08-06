#pragma warning(disable:4786)

#include <vector>
#include <string>
#include <windows.h>
namespace stdwin32 {

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

}