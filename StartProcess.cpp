#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include "StartProcess.h"

#pragma comment(lib, "shell32.lib")

BOOL StartProcess(LPCTSTR lpFile, BOOL bUseCreateProcess)
{
    STARTUPINFO         tStartupInfo = { 0 };
    PROCESS_INFORMATION tProcessInfomation = { 0 };

	LPWSTR pW = wcsdup(lpFile);
    BOOL bResult = CreateProcess(
              NULL
            , pW
            , NULL
            , NULL
            , FALSE
            , 0
            , NULL
            , NULL
            , &tStartupInfo
            , &tProcessInfomation
        );
	free(pW);
    if ( !bResult ) {
        // return( HRESULT_FROM_WIN32( ::GetLastError() ) );
		return FALSE;
    }
 
    ::CloseHandle( tProcessInfomation.hProcess );
    ::CloseHandle( tProcessInfomation.hThread  );

	return TRUE;
}
