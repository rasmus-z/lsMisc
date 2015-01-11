#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>

bool GetFolderIniDir(HINSTANCE hInst, LPTSTR szFolder, DWORD nfSize);
void CreateFolderIniPath(HINSTANCE hInst, LPCTSTR pIniFileName, LPTSTR pOut, LPCTSTR pErrorTemplate);