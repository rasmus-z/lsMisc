#pragma once
BOOL SHMoveOneFile(LPCTSTR lpFileTo, LPCTSTR lpFileFrom, int* pnRet=NULL);
BOOL SHMoveFile(LPCTSTR lpFileTo, const std::vector<std::wstring>& sourcefiles, int* pnRet = NULL);
