#pragma once



#ifdef __cplusplus_cli
void ClearHandle(HANDLE% h);
#else
void ClearHandle(HANDLE& h);
#endif
BOOL IsProcessActive(HANDLE& h);