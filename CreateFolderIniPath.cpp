#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
//#include <string>

#include "GetSpecialFolderPath.h"
#include "CreateFolderIniPath.h"
#include "CreateCompleteDirectory.h"

#include "tstring.h"

bool GetFolderIniDir(HINSTANCE hInst, LPTSTR szFolder, DWORD nfSize,
	LPCTSTR pCreator, LPCTSTR pApp)
{
	TCHAR szT[MAX_PATH];
	if(!GetModuleFileName(NULL, szT, sizeof(szT)/sizeof(TCHAR)))
	{
		throw tstring(_T("Fatal Error"));
	}

	*_tcsrchr(szT, _T('\\'))=0;

	TCHAR szI[MAX_PATH];
	wsprintf(szI, _T("%s\\folder.ini"), szT);

	tstring tmpS;
	int pathtype = GetPrivateProfileInt(_T("presettings"),
		_T("pathType"),
		-1,
		szI);
	switch (pathtype)
	{
	case -1:
	case 0:
	{
		if (!GetModuleFileName(NULL, szFolder, nfSize))
		{
			throw tstring(_T("Fatal Error"));
		}
		*_tcsrchr(szFolder, _T('\\')) = 0;
		return true;
	}

	case 1:
	case 2:
	{
		int csidl = pathtype == 1 ? CSIDL_LOCAL_APPDATA : CSIDL_APPDATA;
		if (!GetSpecialFolderPath(NULL, csidl, szFolder))
			return false;

		PathAddBackslash(szFolder);
		lstrcat(szFolder, pCreator);
		lstrcat(szFolder, _T("\\"));
		lstrcat(szFolder, pApp);
		CreateCompleteDirectory(szFolder, NULL);
		return true;
	}
	break;

	case 3:
	{
		if (GetPrivateProfileString(_T("presettings"),
			_T("folder"),
			_T(""),
			szFolder,
			nfSize,
			szI))
		{
			return true;
		}
	}
	break;
	}
	
	return false;
}

void CreateFolderIniPath_obsoletes(HINSTANCE hInst,
	LPCTSTR pIniFileName, 
	LPTSTR pOut, 
	LPCTSTR pCreator,
	LPCTSTR pApp,
	LPCTSTR pErrorTemplate)
{
	TCHAR szT[MAX_PATH];
	TCHAR szFolder[MAX_PATH];
	
	if(GetFolderIniDir(hInst, 
		szFolder,
		(sizeof(szFolder)/sizeof(TCHAR))-sizeof(TCHAR),
		pCreator,
		pApp)
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
		if(!GetModuleFileName(NULL, szT, sizeof(szT)/sizeof(TCHAR)))
		{
			throw tstring(_T("Fatal Error"));
		}

		*_tcsrchr(szT, _T('\\'))=0;
		wsprintf(pOut, _T("%s\\%s"), szT, pIniFileName);
	}
}
