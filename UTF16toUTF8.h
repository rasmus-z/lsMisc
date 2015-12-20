BYTE* UTF16toUTF8(LPCWSTR pIN);
LPWSTR UTF16_convertEndian(LPCWSTR pIN);
LPWSTR UTF8toUTF16(const LPBYTE pIN);
bool UTF8toUTF16(const LPBYTE pIN, std::wstring& w);