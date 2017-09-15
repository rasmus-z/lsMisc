#pragma once

#include <vector>
#include <string>

namespace Ambiesoft {

	BOOL SHMoveFile(LPCTSTR lpFileTo, LPCTSTR lpzzFileFrom, int* pnRet = NULL);
	BOOL SHCopyFile(LPCTSTR lpFileTo, LPCTSTR lpFileFrom, int* pnRet = NULL);
	BOOL SHDeleteFile(LPCTSTR lpFile, int* pnRet = NULL);

	BOOL SHMoveFile(LPCTSTR lpFileTo, const std::vector<std::wstring>& sourcefiles, int* pnRet = NULL);
	BOOL SHCopyFile(LPCTSTR lpFileTo, const std::vector<std::wstring>& sourcefiles, int* pnRet = NULL);
	BOOL SHDeleteFile(const std::vector<std::wstring>& files, int* pnRet = NULL);

	BOOL SHMoveFile(const std::vector<std::wstring>& destfiles, const std::vector<std::wstring>& sourcefiles, int* pnRet = NULL);
	BOOL SHCopyFile(const std::vector<std::wstring>& destfiles, const std::vector<std::wstring>& sourcefiles, int* pnRet = NULL);
	
}