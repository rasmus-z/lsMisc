#ifdef UNICODE
#define IsFileExists IsFileExistsW
#else
#define IsFileExists IsFileExistsA
#endif
BOOL IsFileExistsW(LPCWSTR pPath);
BOOL IsFileExistsA(LPCSTR pPath);