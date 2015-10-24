#pragma once

// HOW TO USE
// 1. Include I18N.h and I18N.cpp.
// 2. Call i18nInitLangmap() at startup, or i18nInitLangmap(L"jpn") for specifying language.
// 3. Create a Folder named "lang" under the exe-residing-folder, and place a lang file in it 
//    and name it like "jpn.txt".
// 4. jpn.txt format is like follows:
//    "Hello"="‚±‚ñ‚É‚¿‚Í"
//    "Goodby"="‚³‚æ‚¤‚È‚ç"
// 5. Call I18N(L"Hello") in your code. It returns const "‚±‚ñ‚É‚¿‚Í".
// 6. Call i18Nxxxx() for translating windows texts. Show the source for more informations.
// 7. Look the debugger output when the program exists. Untranslated words will be printed.

namespace Ambiesoft {
LPCTSTR i18nInitLangmap(HINSTANCE hInst=NULL, LPCTSTR pLang = NULL);
LPCTSTR i18nGetCurrentLang();
LPCWSTR I18N(LPCWSTR pIN);
void i18nChangeWindowTextW(HWND hwnd);
void i18nChangeChildWindowTextW(HWND hwnd);
void i18nChangeMenuTextW(HMENU menu);

#ifdef UNICODE
inline void i18nChangeWindowText(HWND hwnd){ i18nChangeWindowTextW(hwnd); }
inline void i18nChangeChildWindowText(HWND hDlg){ i18nChangeChildWindowTextW(hDlg); }
inline void i18nChangeMenuText(HMENU menu) { i18nChangeMenuTextW(menu); }
#endif

} // namespace Ambiesoft