#include "tstring.h"


BOOL GetClipboardTextA(HWND hWnd, std::string& strData);
BOOL GetClipboardTextW(HWND hWnd, std::wstring& strData);

#ifdef UNICODE
#define GetClipboardText GetClipboardTextW
#else
#define GetClipboardText GetClipboardTextA
#endif