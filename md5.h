

DWORD md5A(HANDLE hFile, std::string& ret);
DWORD md5A(LPCTSTR filename, std::string& ret);

DWORD md5W(HANDLE hFile, std::wstring& ret);
DWORD md5W(LPCTSTR filename, std::wstring& ret);

DWORD string2hashA(const char *stro);

DWORD FileHandleToDwordHash(HANDLE hFile, DWORD* hash);