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
