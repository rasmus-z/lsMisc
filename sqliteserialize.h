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