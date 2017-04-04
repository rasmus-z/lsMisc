#pragma once

BYTE* UTF16toMultiByte(UINT cp, LPCWSTR pIN, size_t* pLenOut=NULL);
BYTE* UTF16toUTF8(LPCWSTR pIN);
LPWSTR UTF16_convertEndian(LPCWSTR pIN);
LPWSTR MultiBytetoUTF16(UINT cp, const LPBYTE pIN, size_t cbLen=-1);
LPWSTR UTF8toUTF16(const LPBYTE pIN);
bool UTF8toUTF16(const LPBYTE pIN, std::wstring& w);
#ifdef __cplusplus_cli  
BYTE* UTF16toUTF8(System::String^ s);
#endif //__cplusplus_cli  