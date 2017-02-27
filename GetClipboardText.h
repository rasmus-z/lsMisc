#include "tstring.h"


BOOL GetClipboardTextA(HWND hWnd, std::string& strData);
BOOL GetClipboardTextW(HWND hWnd, std::wstring& strData);

BOOL GetClipboardHtmlA(HWND hWnd, std::string& strData);
BOOL GetClipboardHtmlW(HWND hWnd, std::wstring& strData);

#ifdef UNICODE
#define GetClipboardText GetClipboardTextW
#define GetClipboardHtml GetClipboardHtmlW
#else
#define GetClipboardText GetClipboardTextA
#define GetClipboardHtml GetClipboardHtmlA
#endif