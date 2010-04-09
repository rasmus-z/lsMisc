#include <tchar.h>
#include <windows.h>
#include "IsTextFileExtension.h"

BOOL IsTextFileExtension(LPCTSTR lpcExtention)
{
	LPTSTR lpExtension = _tcsdup(lpcExtention);
	_tcslwr(lpExtension);

	BOOL bRet =  _tcsstr(lpExtension, _T("htm"))!=0 
				|| lstrcmpi(lpExtension, _T(".txt")) ==0
				|| lstrcmpi(lpExtension, _T(".pl")) ==0
				|| lstrcmpi(lpExtension, _T(".asp")) ==0
				|| lstrcmpi(lpExtension, _T(".cgi")) ==0;

	free(lpExtension);
	return bRet;
}
