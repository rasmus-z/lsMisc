#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>

#include "GetFileNameFromHwnd.h"

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
        BOOL (WINAPI *lpfEnumProcessModules)
                            (HANDLE, HMODULE*, DWORD, LPDWORD);
        DWORD (WINAPI *lpfGetModuleFileNameEx)
                            (HANDLE, HMODULE, LPTSTR, DWORD);

        HINSTANCE hInstLib = LoadLibrary(_T("PSAPI.DLL"));
        if ( hInstLib == NULL )
            return FALSE ;

        lpfEnumProcessModules = (BOOL(WINAPI *)
            (HANDLE, HMODULE *, DWORD, LPDWORD))GetProcAddress(
                        hInstLib, "EnumProcessModules");
#ifdef UNICODE
        lpfGetModuleFileNameEx =(DWORD (WINAPI *)
            (HANDLE, HMODULE, LPWSTR, DWORD))GetProcAddress(
                        hInstLib, "GetModuleFileNameExW");
#else
        lpfGetModuleFileNameEx =(DWORD (WINAPI *)
            (HANDLE, HMODULE, LPSTR, DWORD))GetProcAddress(
                        hInstLib, "GetModuleFileNameExA");
#endif
        if ( lpfEnumProcessModules && lpfGetModuleFileNameEx )
        {
            HANDLE hProcess;
            hProcess = OpenProcess(
                    PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                    FALSE, dwProcessId);
            if ( hProcess )
            {
                HMODULE hModule;
                DWORD dwNeed;
                if (lpfEnumProcessModules(hProcess,
                            &hModule, sizeof(hModule), &dwNeed))
                {
                    if ( lpfGetModuleFileNameEx(hProcess, hModule,
                                            lpszFileName, nSize) )
                        bResult = TRUE;
                }
                CloseHandle( hProcess ) ;
            }
        }
        FreeLibrary( hInstLib ) ;
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