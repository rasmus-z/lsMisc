#include "stdafx.h"
#include <windows.h>

#include "GetHotkeyString.h"

using namespace std;

wstring GetHotkeyStringW(WORD wKey)
{
	wstring ret;
	if(wKey==0)
		return ret;

	if(HIBYTE(wKey) & MOD_CONTROL)
		ret += L"Ctrl+";
	if(HIBYTE(wKey) & MOD_SHIFT)
		ret += L"Shift+";
//	if(HIBYTE(wKey) & MOD_NOREPEAT)
//		ret += L"NoRepeat+";
	if(HIBYTE(wKey) & MOD_WIN)
		ret += L"Win+";
	if(HIBYTE(wKey) & MOD_ALT)
		ret += L"Alt+";

	unsigned int scanCode = MapVirtualKey(LOBYTE(wKey), 0); //MAPVK_VK_TO_VSC);

    // because MapVirtualKey strips the extended bit for some keys
    switch (wKey)
    {
        case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN: // arrow keys
        case VK_PRIOR: case VK_NEXT: // page up and page down
        case VK_END: case VK_HOME:
        case VK_INSERT: case VK_DELETE:
        case VK_DIVIDE: // numpad slash
        case VK_NUMLOCK:
        {
            scanCode |= 0x100; // set extended bit
            break;
        }
    }

    wchar_t keyName[64] = {0};
    if (GetKeyNameText(scanCode << 16, keyName, sizeof(keyName)/sizeof(keyName[0]) != 0))
    {
        ret += keyName;
    }
    else
    {
        ret += L"[Error]";
    }


	return ret;
}
