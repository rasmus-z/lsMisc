#include "tstring.h"

BOOL TrxIsRegKeyExists(HKEY hRoot, LPCTSTR pSub);
BOOL TrxRegGetValue(HKEY hRoot, LPCTSTR pSub, LPCTSTR pName, tstring& value);
