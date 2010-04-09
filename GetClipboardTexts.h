#include "tstring.h"


BOOL GetClipboardTextsA(HWND hWnd, std::string& strData);
BOOL GetClipboardTextsW(HWND hWnd, std::wstring& strData);

#ifdef UNICODE
#define GetClipboardTexts GetClipboardTextsW
#else
#define GetClipboardTexts GetClipboardTextsA
#endif