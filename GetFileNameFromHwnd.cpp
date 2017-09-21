//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>

#include "GetFileNameFromHwnd.h"

static BOOL (WINAPI *lpfEnumProcessModules)
                    (HANDLE, HMODULE*, DWORD, LPDWORD);
static DWORD (WINAPI *lpfGetModuleFileNameEx)
                    (HANDLE, HMODULE, LPTSTR, DWORD);
typedef BOOL (WINAPI *LPFEnumProcessModulesEx)
					(HANDLE, HMODULE*, DWORD, LPDWORD, DWORD);
static LPFEnumProcessModulesEx lpfEnumProcessModulesEx;

typedef BOOL (WINAPI *LPFQueryFullProcessImageNameW)(
	HANDLE hProcess,
	DWORD  dwFlags,
	LPTSTR lpExeName,
	PDWORD lpdwSize
	);
static LPFQueryFullProcessImageNameW lpfQueryFullProcessImageNameW;

static bool init()
{
	static bool initted = false;
	static bool ok=false;
	if(initted)
		return ok;
	initted=true;

	static HINSTANCE sthPSAPI;
	if(!sthPSAPI)
		sthPSAPI = LoadLibrary(_T("PSAPI.DLL"));
    if ( sthPSAPI == NULL )
		return ok=false;

	static HINSTANCE sthKernel;
	if (!sthKernel)
		sthKernel = LoadLibrary(_T("kernel32.dll"));
	if (sthKernel == NULL)
		return ok = false;

	lpfEnumProcessModulesEx = (LPFEnumProcessModulesEx)GetProcAddress(
		sthPSAPI, "EnumProcessModulesEx");

    lpfEnumProcessModules = (BOOL(WINAPI *)
        (HANDLE, HMODULE *, DWORD, LPDWORD))GetProcAddress(
                    sthPSAPI, "EnumProcessModules");
	if(lpfEnumProcessModules==NULL)
		return ok=false;
#ifdef UNICODE
    lpfGetModuleFileNameEx =(DWORD (WINAPI *)
        (HANDLE, HMODULE, LPWSTR, DWORD))GetProcAddress(
                    sthPSAPI, "GetModuleFileNameExW");
	if(lpfGetModuleFileNameEx==NULL)
		return ok=false;
#else
    lpfGetModuleFileNameEx =(DWORD (WINAPI *)
        (HANDLE, HMODULE, LPSTR, DWORD))GetProcAddress(
                    sthPSAPI, "GetModuleFileNameExA");
	if(lpfGetModuleFileNameEx==NULL)
		return ok=false;
#endif

	lpfQueryFullProcessImageNameW = (LPFQueryFullProcessImageNameW)GetProcAddress(
		sthKernel, "QueryFullProcessImageNameW");
	return ok=true;
}

#ifndef LIST_MODULES_ALL
#define LIST_MODULES_ALL 0x03
#endif

static BOOL getModuleFromHProcess(HANDLE hProcess, HMODULE* phModule)
{
	DWORD dwNeed = 0;
	if (lpfEnumProcessModulesEx)
		return lpfEnumProcessModulesEx(hProcess, phModule, sizeof(*phModule), &dwNeed, LIST_MODULES_ALL);

	return lpfEnumProcessModules(hProcess, phModule, sizeof(*phModule), &dwNeed);
}

BOOL GetFileNameFromHwnd(HWND hWnd, LPTSTR lpszFileName, DWORD nSize)
{
    BOOL bResult = FALSE;

    DWORD dwProcessId;
    GetWindowThreadProcessId(hWnd , &dwProcessId);

    OSVERSIONINFO osverinfo;
    osverinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if ( !GetVersionEx(&osverinfo) )
        return FALSE;


    if ( osverinfo.dwPlatformId == VER_PLATFORM_WIN32_NT )
    {
        if ( init() )
        {
            HANDLE hProcess;
            hProcess = OpenProcess(
                    PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                    FALSE, dwProcessId);
            if ( hProcess )
            {
				if (lpfQueryFullProcessImageNameW)
				{
					DWORD dwSize = nSize;
					bResult = lpfQueryFullProcessImageNameW(hProcess,
						0,
						lpszFileName,
						&dwSize);
				}
				else
				{
					HMODULE hModule;
					if (getModuleFromHProcess(hProcess, &hModule))
					{
						if (lpfGetModuleFileNameEx(hProcess, hModule, lpszFileName, nSize))
							bResult = TRUE;
					}
				}
                CloseHandle( hProcess ) ;
            }
        }
        //FreeLibrary( hInstLib ) ;
    }
#if !UNICODE
    else if ( osverinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
    {
        HANDLE (WINAPI *lpfCreateSnapshot)(DWORD, DWORD);
        BOOL (WINAPI *lpfProcess32First)(HANDLE, LPPROCESSENTRY32);
        BOOL (WINAPI *lpfProcess32Next)(HANDLE, LPPROCESSENTRY32);
        
        lpfCreateSnapshot =
            (HANDLE(WINAPI*)(DWORD,DWORD))GetProcAddress(
                                GetModuleHandleA("kernel32.dll"),
                                "CreateToolhelp32Snapshot" );
         lpfProcess32First=
            (BOOL(WINAPI*)(HANDLE,LPPROCESSENTRY32))GetProcAddress(
                                GetModuleHandleA("kernel32.dll"),
                                "Process32First" );
         lpfProcess32Next=
            (BOOL(WINAPI*)(HANDLE,LPPROCESSENTRY32))GetProcAddress(
                                GetModuleHandleA("kernel32.dll"),
                                "Process32Next" );
        if ( !lpfCreateSnapshot ||
             !lpfProcess32First ||
             !lpfProcess32Next)
            return FALSE;

        HANDLE hSnapshot;
        hSnapshot = lpfCreateSnapshot(TH32CS_SNAPPROCESS , 0);
        if (hSnapshot != (HANDLE)-1)
        {
            PROCESSENTRY32 pe;
            pe.dwSize = sizeof(PROCESSENTRY32);
            if ( lpfProcess32First(hSnapshot, &pe) )
            {
                do {
                    if (pe.th32ProcessID == dwProcessId)
                    {
                        lstrcpyA(lpszFileName, pe.szExeFile);
                        bResult = TRUE;
                        break;
                    }
                } while ( lpfProcess32Next(hSnapshot, &pe) );
            }

            CloseHandle(hSnapshot);
        }
    }
#endif
 
    return bResult;
}