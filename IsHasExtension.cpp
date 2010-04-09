#include <tchar.h>
#include <windows.h>
#include "IsHasExtension.h"

BOOL IsHasExtension(LPCTSTR pPath, LPCTSTR pExtention)
{
	if ( pPath==NULL || pPath[0]==0 )
		return FALSE;

	LPCTSTR pExt = _tcsrchr(pPath, _T('.'));
	if ( pExt==NULL )
		return FALSE;

	return _tcsicmp(pExt+1, pExtention)==0;
}
