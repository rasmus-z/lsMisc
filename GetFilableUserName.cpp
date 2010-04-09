#include <windows.h>
#include <tchar.h>
#include "GetFilableUserName.h"
#include "ChangeFilenamable.h"

tstring GetFilableUserName()
{
	TCHAR szUser[1024]; szUser[0]=0;
	DWORD d = sizeof(szUser);
	if ( !GetUserName(szUser, &d) )
		return _T("unknown");

	if ( !ChangeFilenamable(szUser) )
		return _T("unknown");

	return szUser;
}
