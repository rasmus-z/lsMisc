static BOOL createDB(LPCWSTR pDBPath, LPCSTR pTableName);

BOOL sqlWritePrivateProfileString(
  LPCTSTR lpAppName,
  LPCTSTR lpKeyName,
  LPCTSTR lpString,
  LPCTSTR lpFileName );
BOOL sqlWritePrivateProfileInt(
  LPCTSTR lpAppName,
  LPCTSTR lpKeyName,
  int nData,
  LPCTSTR lpFileName );

BOOL sqlGetPrivateProfileString(
    LPCTSTR lpAppName,
    LPCTSTR lpKeyName,
    LPCTSTR lpDefault,
    LPTSTR lpReturnedString,
    DWORD nSize,
    LPCWSTR lpFileName);

UINT sqlGetPrivateProfileInt(
    LPCWSTR lpAppName,
    LPCWSTR lpKeyName,
    INT nDefault,
    LPCWSTR lpFileName
    );