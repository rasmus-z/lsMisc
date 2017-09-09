#pragma once
#include "tstring.h"

namespace Ambiesoft {

BOOL browseFolder(HWND hParent, LPCTSTR lpszTitle, LPTSTR pFolder);
BOOL browseFolder(HWND hParent, LPCTSTR lpszTitle, tstring folder);

#ifdef __cplusplus_cli
BOOL browseFolder(System::Windows::Forms::IWin32Window^ win, System::String^ title, System::String^% folder);
#endif

} // namespace