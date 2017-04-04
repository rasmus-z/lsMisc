#pragma once
BOOL browseFolder( HWND hParent, LPCTSTR lpszTitle, LPTSTR pFolder );

#ifdef __cplusplus_cli
BOOL browseFolder(System::Windows::Forms::IWin32Window^ win, System::String^ title, System::String^% folder);
#endif