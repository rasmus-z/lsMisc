BYTE* UTF16toMultiByte(UINT cp, LPCWSTR pIN, size_t* pLenOut=NULL);
BYTE* UTF16toUTF8(LPCWSTR pIN);
LPWSTR UTF16_convertEndian(LPCWSTR pIN);
LPWSTR MultiBytetoUTF16(UINT cp, const LPBYTE pIN);
LPWSTR UTF8toUTF16(const LPBYTE pIN);
bool UTF8toUTF16(const LPBYTE pIN, std::wstring& w);