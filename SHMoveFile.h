#pragma once

#include <vector>
#include <string>

namespace Ambiesoft {

	BOOL SHMoveOneFile(LPCTSTR lpFileTo, LPCTSTR lpzzFileFrom, int* pnRet=NULL);
	BOOL SHCopyOneFile(LPCTSTR lpFileTo, LPCTSTR lpFileFrom, int* pnRet=NULL);

	BOOL SHMoveFile(LPCTSTR lpFileTo, const std::vector<std::wstring>& sourcefiles, int* pnRet = NULL);
	BOOL SHCopyFile(LPCTSTR lpFileTo, const std::vector<std::wstring>& sourcefiles, int* pnRet=NULL);
}