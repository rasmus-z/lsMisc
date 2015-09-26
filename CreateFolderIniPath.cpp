#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
//#include <string>
#include "CreateFolderIniPath.h"
#include "tstring.h"

bool GetFolderIniDir(HINSTANCE hInst, LPTSTR szFolder, DWORD nfSize)
{
	TCHAR szT[MAX_PATH];
	if(!GetModuleFileName(NULL, szT, sizeof(szT)/sizeof(TCHAR)))
	{
		throw tstring(_T("Fatal Error"));
	}

	*_tcsrchr(szT, _T('\\'))=0;

	TCHAR szI[MAX_PATH];
	wsprintf(szI, _T("%s\\folder.ini"), szT);
	if(GetPrivateProfileString(_T("presettings"), 
		_T("folder"),
		_T(""),
		szFolder,
		nfSize,
		szI))
	{
		return true;
	}

	lstrcat(szFolder, szT);
	return false;
}

void CreateFolderIniPath(HINSTANCE hInst, LPCTSTR pIniFileName, LPTSTR pOut, LPCTSTR pErrorTemplate)
{
	TCHAR szT[MAX_PATH];
	TCHAR szFolder[MAX_PATH];
	
	if(GetFolderIniDir(hInst, szFolder, (sizeof(szFolder)/sizeof(TCHAR))-sizeof(TCHAR))
	   && (szFolder[0] != 0 ))
	{
		wsprintf(szT, _T("%s\\%s"), szFolder, pIniFileName);
		if(!PathFileExists(szT))
		{
			TCHAR message[MAX_PATH + 128];
			wsprintf(message, (pErrorTemplate ? pErrorTemplate : _T("%s is not found. Exiting.")), szT);
			throw tstring(message);
		}
		lstrcpy(pOut, szT);
	}
	else
	{
//		if(!GetModuleFileName(NULL, szT, sizeof(szT)/sizeof(TCHAR)))
//		{
//			throw tstring(_T("Fatal Error"));
//		}

//		*_tcsrchr(szT, _T('\\'))=0;
		wsprintf(pOut, _T("%s\\%s"), szFolder, pIniFileName);
	}
}
