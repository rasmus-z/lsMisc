#pragma once

namespace Ambiesoft {
void initLangmap(LPCWSTR pLang = NULL);
LPCWSTR I18N(LPCWSTR pIN);
void i18nChangeWindowText(HWND hwnd);
void i18nChangeChildWindowText(HWND hwnd);
void i18nChangeMenuText(HMENU menu);

} // namespace Ambiesoft