#pragma once

// HOW TO USE
// 1. Include I18N.h and I18N.cpp to your project.
// 2. Call i18nInitLangmap() at startup, or i18nInitLangmap(L"jpn") for specifying language.
// 3. Create a Folder named "lang" in the same folder your executable resides, and place a lang file in it 
//    and name it like "myapp.jpn.txt" where myapp is name of the application.
// 4. "myapp.jpn.txt" format is like follows:
//    "Hello"="‚±‚ñ‚É‚¿‚Í"
//    "Goodby"="‚³‚æ‚¤‚È‚ç"
// 5. Call I18N(L"Hello") in your code. It returns const "‚±‚ñ‚É‚¿‚Í".
// 6. Call i18NChangexxxx() for translating windows or dialog texts. Show the source for more informations.
// 7. Look the debugger output when the program exists. Untranslated words will be printed.

namespace Ambiesoft {
	LPCWSTR i18nInitLangmap(HINSTANCE hInst=NULL, LPCWSTR pLang = NULL, LPCWSTR pAppName=NULL);
	LPCWSTR i18nGetCurrentLang();

	LPCWSTR I18NW(LPCWSTR pIN);
#ifdef UNICODE
	inline LPCWSTR I18N(LPCWSTR pIN) { return I18NW(pIN); }
#else
	inline LPCSTR I18N(LPCSTR pIN) { return I18NA(pIN); }
#endif

	void i18nChangeWindowText(HWND hwnd);
	void i18nChangeChildWindowText(HWND hwnd);
	void i18nChangeMenuText(HMENU menu);
} // namespace Ambiesoft
