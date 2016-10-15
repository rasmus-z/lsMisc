#include "stdafx.h"
#include <windows.h>
#include "IsWindowsNT.h"

BOOL IsWindowsNT(void)
{
    static BOOL once = FALSE;
    static BOOL isNT = FALSE;

    if (!once)
    {
        OSVERSIONINFO osver;
        osver.dwOSVersionInfoSize = sizeof(osver);
        if (GetVersionEx(&osver))
            if (osver.dwPlatformId == VER_PLATFORM_WIN32_NT)
                isNT = TRUE;
        once = TRUE;
    }
    return isNT;
}

BOOL IsWinXPOrHigher()
{
	
	OSVERSIONINFO osver;
    osver.dwOSVersionInfoSize = sizeof(osver);
    if (!GetVersionEx(&osver))
		return FALSE;
    
	// return (OS.Platform == PlatformID.Win32NT) && ((OS.Version.Major > 5) || ((OS.Version.Major == 5) && (OS.Version.Minor >= 1)));
	
	return (osver.dwPlatformId==VER_PLATFORM_WIN32_NT) && 
		(  (osver.dwMajorVersion > 5) || ( (osver.dwMajorVersion==5) && (osver.dwMinorVersion >= 1) )  );

}

BOOL IsWinVistaOrHigher()
{
	OSVERSIONINFO osver;
    osver.dwOSVersionInfoSize = sizeof(osver);
    if (!GetVersionEx(&osver))
		return FALSE;

    // return (OS.Platform == PlatformID.Win32NT) && (OS.Version.Major >= 6);

	return (osver.dwPlatformId==VER_PLATFORM_WIN32_NT) && (osver.dwMajorVersion >= 6);
}