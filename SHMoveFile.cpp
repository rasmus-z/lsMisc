#include "stdafx.h"

#include <windows.h>
#include <tchar.h>
#include <assert.h>

#include <vector>
#include <string>

#include <stlsoft/smartptr/scoped_handle.hpp>

using std::vector;
using std::wstring;
typedef vector<wstring> STRINGVECTOR;

#include "SHMoveFile.h"



static BOOL SHMoveFile(LPCTSTR lpFileTo, LPCTSTR lpFileFrom, int* pnRet)
{
	assert(lpFileTo && lpFileFrom);

	//if( GetFileAttributes(lpFileFrom)==0xFFFFFFFF )
	//	return FALSE;

	size_t destCharCount = wcslen(lpFileTo);
	size_t destByteAlloc = (destCharCount + 1 + 1) * sizeof(TCHAR);
	LPTSTR pTo = (LPTSTR)LocalAlloc(LMEM_ZEROINIT, destByteAlloc);
	stlsoft::scoped_handle<void*> ha(pTo, LocalFree);

	wcscpy_s(pTo, destByteAlloc, lpFileTo);

	SHFILEOPSTRUCT sfo = { 0 };
	sfo.hwnd = NULL;
	sfo.wFunc = FO_MOVE;
	sfo.pFrom = lpFileFrom;
	sfo.pTo = pTo;
	sfo.fFlags = FOF_ALLOWUNDO | FOF_FILESONLY | FOF_NOERRORUI;

	int nRet = SHFileOperation(&sfo);//&& !sfo.fAnyOperationsAborted;
	if (pnRet)
		*pnRet = nRet;
	// it seems SHFileOperation returns 113 when same path specified.
	return nRet == 0 || nRet == 113;
}

BOOL SHMoveOneFile(LPCTSTR lpFileTo, LPCTSTR lpFileFrom, int* pnRet)
{
	assert(lpFileTo && lpFileFrom);

	if (GetFileAttributes(lpFileFrom) == 0xFFFFFFFF)
		return FALSE;

	size_t fromCharCount = wcslen(lpFileFrom);
	LPTSTR pFrom = (LPTSTR)LocalAlloc(LMEM_ZEROINIT, (fromCharCount + 1 + 1) * sizeof(TCHAR));
	stlsoft::scoped_handle<void*> ha(pFrom, LocalFree);
	wcscpy_s(pFrom, fromCharCount, lpFileTo);

	return SHMoveFile(lpFileTo, pFrom, pnRet);
}
BOOL SHMoveFile(LPCTSTR lpFileTo, const std::vector<std::wstring>& sourcefiles, int* pnRet)
{
	size_t size = 0;
	for (STRINGVECTOR::const_iterator it = sourcefiles.begin(); it != sourcefiles.end(); ++it)
	{
		size += (it->size() + 1) * sizeof(WCHAR);
	}

	size += sizeof(WCHAR);
	BYTE* p = (BYTE*)LocalAlloc(0, size);
	stlsoft::scoped_handle<void*> ha(p, LocalFree);
	LPCTSTR pStart = (LPCTSTR)p;
	for (STRINGVECTOR::const_iterator it = sourcefiles.begin(); it != sourcefiles.end(); ++it)
	{
		size_t copysize = (it->size()+1) * sizeof(WCHAR);
		errno_t err = memcpy_s(p, size, it->c_str(), copysize);
		if (err != 0)
		{
			if (pnRet)
				*pnRet = err;
			return FALSE;
		}
		p += copysize;
		size -= copysize;
	}
	*p = 0;


	return SHMoveFile(lpFileTo, pStart, pnRet);

}