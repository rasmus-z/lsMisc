// static BOOL createDB(LPCWSTR pDBPath, LPCSTR pTableName);

BOOL sqlWritePrivateProfileString(
  LPCWSTR lpAppName,
  LPCWSTR lpKeyName,
	LPCWSTR lpString,
	LPCWSTR lpFileName );
BOOL sqlWritePrivateProfileInt(
	LPCWSTR lpAppName,
	LPCWSTR lpKeyName,
  int nData,
	LPCWSTR lpFileName );

BOOL sqlGetPrivateProfileString(
	LPCWSTR lpAppName,
	LPCWSTR lpKeyName,
	LPCWSTR lpDefault,
	LPWSTR lpReturnedString,
    DWORD nSize,
	LPCWSTR lpFileName);
UINT sqlGetPrivateProfileInt(
	LPCWSTR lpAppName,
	LPCWSTR lpKeyName,
    INT nDefault,
	LPCWSTR lpFileName
    );


BOOL sqlGetPrivateProfileStringArray(LPCWSTR pApp, LPCWSTR pKey, std::vector<std::wstring>& ss, LPCWSTR pIni);
BOOL sqlWritePrivateProfileStringArray(LPCWSTR pApp, LPCWSTR pKey, std::vector<std::wstring>& ss, LPCWSTR pIni);