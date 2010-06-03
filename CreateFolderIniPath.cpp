#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <string>
#include "CreateFolderIniPath.h"

void CreateFolderIniPath(HINSTANCE hInst, LPTSTR pOut)
{
	char* lpszExt;
	char szT[MAX_PATH];

	if(!GetModuleFileName(NULL, szT, sizeof(szT)))
	{
		throw std::string("Fatal Error");
	}

	lpszExt    = strrchr(szT, '\\');
	*lpszExt = 0;

	char szFolder[MAX_PATH];
	char szI[MAX_PATH];
	wsprintf(szI, "%s\\folder.ini", szT);
	GetPrivateProfileString("presettings", 
		"folder",
		"",
		szFolder,
		sizeof(szFolder)-1,
		szI);

	if(szFolder[0] != 0 )
	{
		wsprintf(szT, "%s\\CBRevamper.ini", szFolder);
		if(!PathFileExists(szT))
		{
			char message[MAX_PATH + 128];
			wsprintf(message, "%s is not found. Exiting.", szT);
			throw std::string(message);
		}
	}
	else
	{
		wsprintf(pOut, "%s\\CBRevamper.ini", szT);
	}
}
