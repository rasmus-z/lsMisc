#include <windows.h>

#include "ConfirmErrorRetry.h"

BOOL ConfirmErrorRetry(BOOL bOK, HWND hWnd, LPCTSTR pMessage, LPCTSTR pTitle)
{
	if ( bOK )
		return TRUE;

	return IDYES == MessageBox(hWnd, pMessage, pTitle, MB_ICONEXCLAMATION|MB_YESNO);
}