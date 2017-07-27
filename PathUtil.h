#pragma once
BOOL myPathIsRoot(LPCTSTR pPath);
BOOL IsBothEmptyOrNULL(LPCTSTR p1, LPCTSTR p2);
BOOL myPathSameDepth(LPCTSTR p1, LPCTSTR p2);
BOOL myPathIsChildIncluded(LPCTSTR p1, LPCTSTR p2, std::wstring* pDupPath=NULL);