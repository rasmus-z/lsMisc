// #include "tstring.h" enable this after string->tstring
// #include <string>
namespace Ambiesoft {
	BOOL SetClipboardTextA(HWND hWnd, LPCSTR pSTR);
	BOOL SetClipboardTextW(HWND hWnd, LPCWSTR pSTR);

#ifdef UNICODE
#define SetClipboardText SetClipboardTextW
#else
#define SetClipboardText SetClipboardTextA
#endif

}