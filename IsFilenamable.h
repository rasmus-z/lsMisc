#pragma once

#ifndef _STRING_
#pragma error("Include string")
#endif
std::wstring GetFileInamableChars();
std::wstring GetRelativePathInamableChars();
std::wstring GetFullPathInamableChars();
#ifdef __cplusplus_cli
System::String^ GetFullPathInamableCharsCLR();
System::String^ GetFileInamableCharsCLR();
#endif

BOOL makeFileNamable(LPTSTR pch);

BOOL IsFileNamble(LPCTSTR p);
BOOL IsRelativePathNamble(LPCTSTR p);
BOOL IsFullPathNamble(LPCTSTR p);

