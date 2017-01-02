
#include "stdafx.h"

#include <Shlobj.h>
#include <string>
#include <vector>
#include "SetCSIDLtoEnv.h"

using namespace std;

static BOOL mySetEnvironmentVariable(LPCWSTR pPrefix, LPCWSTR pSuffix, LPCWSTR p1, LPCWSTR p2, std::vector<wstring>* pResult)
{

	wstring w1;
	if(pPrefix)
		w1 = pPrefix;
	w1 += p1;
	if(pSuffix)
		w1 += pSuffix;

	TRACE(w1.c_str());
	TRACE(L"=");
	TRACE(p2);
	TRACE(L"\r\n");


	BOOL bRet = SetEnvironmentVariable(w1.c_str(),p2);
	if(bRet && pResult)
	{
		pResult->push_back(w1);
	}
	return bRet;
}

#define SETENVSPFOLDER(PPP,TTT, SSS,RRR) do {\
		if(SHGetSpecialFolderPath(NULL,szT, CSIDL_##TTT, FALSE)) {\
			VERIFY(mySetEnvironmentVariable(PPP,SSS, L#TTT, szT,RRR)); \
		}\
	} while(false) 


void SetCSIDLtoEnvW(LPCWSTR pPrefix, LPCWSTR pSuffix, std::vector<wstring>* pResult)
{
	WCHAR szT[MAX_PATH];

	SETENVSPFOLDER(	pPrefix,	DESKTOP	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	INTERNET	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PROGRAMS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	CONTROLS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PRINTERS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PERSONAL	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	FAVORITES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	STARTUP	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	RECENT	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	SENDTO	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	BITBUCKET	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	STARTMENU	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	MYDOCUMENTS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	MYMUSIC	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	MYVIDEO	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	DESKTOPDIRECTORY	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	DRIVES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	NETWORK	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	NETHOOD	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	FONTS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	TEMPLATES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_STARTMENU	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_PROGRAMS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_STARTUP	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_DESKTOPDIRECTORY	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	APPDATA	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PRINTHOOD	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	LOCAL_APPDATA	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	LOCAL_APPDATA	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	ALTSTARTUP	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_ALTSTARTUP	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_FAVORITES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	INTERNET_CACHE	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COOKIES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	HISTORY	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_APPDATA	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	WINDOWS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	SYSTEM	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PROGRAM_FILES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	MYPICTURES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PROFILE	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	SYSTEMX86	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PROGRAM_FILESX86	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PROGRAM_FILES_COMMON	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PROGRAM_FILES_COMMONX86	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_TEMPLATES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_DOCUMENTS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_ADMINTOOLS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	ADMINTOOLS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	CONNECTIONS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_MUSIC	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_PICTURES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_VIDEO	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	RESOURCES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	RESOURCES_LOCALIZED	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_OEM_LINKS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	CDBURN_AREA	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMPUTERSNEARME	,pSuffix	,pResult	);

}