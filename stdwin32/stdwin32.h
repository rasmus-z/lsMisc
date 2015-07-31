#include <vector>
#include <string>
namespace stdwin32 {

	std::wstring stdGetModuleFileName(HINSTANCE hInst=NULL);
	BOOL stdIsFullPath(LPCWSTR pD);
	std::wstring stdCombinePath(LPCWSTR pD1, LPCWSTR pD2);
	std::wstring stdGetParentDirectory(LPCWSTR pPath);
	std::vector<std::wstring> stdSplitSCedPath(LPCWSTR pPath);
}