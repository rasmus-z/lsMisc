#include <windows.h>
#include <tchar.h>
#include "StartProcess.h"

BOOL StartProcess(LPCTSTR lpFile, BOOL bUseCreateProcess)
{
	HINSTANCE h = ShellExecute(NULL, NULL, lpFile, NULL, NULL, SW_SHOW);
	return h > (HINSTANCE)32;
}
