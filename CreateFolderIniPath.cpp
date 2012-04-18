#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <string>
#include "CreateFolderIniPath.h"
#include "tstring.h"

void CreateFolderIniPath(HINSTANCE hInst, LPTSTR pOut)
{
	TCHAR* lpszExt;
	TCHAR szT[MAX_PATH];

	if(!GetModuleFileName(NULL, szT, sizeof(szT)))
	{
		throw tstring(_T("Fatal Error"));
	}

	lpszExt    = _tcsrchr(szT, _T('\\'));
	*lpszExt = 0;

	TCHAR szFolder[MAX_PATH];
	TCHAR szI[MAX_PATH];
	wsprintf(szI, _T("%s\\folder.ini"), szT);
	GetPrivateProfileString(_T("presettings"), 
		_T("folder"),
		_T(""),
		szFolder,
		sizeof(szFolder)-1,
		szI);

	if(szFolder[0] != 0 )
	{
		wsprintf(szT, _T("%s\\CBRevamper.ini"), szFolder);
		if(!PathFileExists(szT))
		{
			TCHAR message[MAX_PATH + 128];
			wsprintf(message, _T("%s is not found. Exiting."), szT);
			throw tstring(message);
		}
		lstrcpy(pOut, szT);
	}
	else
	{
		wsprintf(pOut, _T("%s\\CBRevamper.ini"), szT);
	}
}
