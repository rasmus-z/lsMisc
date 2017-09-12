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

#pragma comment(lib, "Shell32.lib")

namespace Ambiesoft {

	static void* myAlloc(size_t size)
	{
		return calloc(1, size);
	}
	static void myFree(void* p)
	{
		free(p);
	}

	enum CopyORMove {
		CopyORMove_Copy,
		CopyORMove_Move,
	};

	static BOOL SHCopyOrMoveFileImpl(CopyORMove copyormove, bool ismultidest, LPCTSTR lpzzFileTo, LPCTSTR lpzzFileFrom, int* pnRet=NULL)
	{
		SHFILEOPSTRUCT sfo = { 0 };
		sfo.hwnd = NULL;
		sfo.wFunc = copyormove == CopyORMove_Copy ? FO_COPY : FO_MOVE;
		sfo.pFrom = lpzzFileFrom;
		sfo.pTo = lpzzFileTo;
		sfo.fFlags =
			FOF_ALLOWUNDO |
			FOF_FILESONLY |
			FOF_NOERRORUI |
			(ismultidest ? FOF_MULTIDESTFILES : 0) |
			0;

		int nRet = SHFileOperation(&sfo);//&& !sfo.fAnyOperationsAborted;
		if (pnRet)
			*pnRet = nRet;
		// it seems SHFileOperation returns 113 when same path specified.
		return nRet == 0 || nRet == 113;
	}



	static LPTSTR CreateDNString(const vector<wstring>& files, int* pnRet)
	{
		size_t size = 0;
		for (STRINGVECTOR::const_iterator it = files.begin(); it != files.end(); ++it)
		{
			size += (it->size() + 1) * sizeof(WCHAR);
		}

		size += sizeof(WCHAR);
		BYTE* p = (BYTE*)myAlloc(size);
		LPTSTR pStart = (LPTSTR)p;
		for (STRINGVECTOR::const_iterator it = files.begin(); it != files.end(); ++it)
		{
			size_t copysize = (it->size()+1) * sizeof(WCHAR);
			errno_t err = memcpy_s(p, size, it->c_str(), copysize);
			if (err != 0)
			{
				if (pnRet)
					*pnRet = err;
				myFree((void*)pStart);
				return NULL;
			}
			p += copysize;
			size -= copysize;
		}
		*p = 0;
		return pStart;
	}
	static LPTSTR CreateDNString(LPCWSTR pW, int* pnRet)
	{
		vector<wstring> vs;
		vs.push_back(pW);
		return CreateDNString(vs, pnRet);
	}

	
	static BOOL SHCopyOrMoveFile(CopyORMove cm, LPCTSTR lpFileTo, LPCTSTR lpFileFrom, int* pnRet)
	{
		assert(lpFileTo && lpFileFrom);
		if (!(lpFileTo && lpFileFrom))
			return FALSE;

		LPTSTR pFrom = CreateDNString(lpFileFrom, pnRet);
		stlsoft::scoped_handle<void*> ha(pFrom, myFree);
		
		LPTSTR pTo = CreateDNString(lpFileTo, pnRet);
		stlsoft::scoped_handle<void*> hi(pTo, myFree);

		return SHCopyOrMoveFileImpl(cm, false, pTo, pFrom, pnRet);
	}
	BOOL SHMoveFile(LPCTSTR lpFileTo, LPCTSTR lpFileFrom, int* pnRet)
	{
		return SHCopyOrMoveFile(CopyORMove_Move, lpFileTo, lpFileFrom, pnRet);
	}
	BOOL SHCopyFile(LPCTSTR lpFileTo, LPCTSTR lpFileFrom, int* pnRet)
	{
		return SHCopyOrMoveFile(CopyORMove_Copy, lpFileTo, lpFileFrom, pnRet);
	}




	// Move many file to 1 folder
	static BOOL SHCopyOrMoveFile(CopyORMove cm, LPCTSTR lpFileTo, const std::vector<std::wstring>& sourcefiles, int* pnRet)
	{
		LPTSTR pFroms = CreateDNString(sourcefiles, pnRet);
		stlsoft::scoped_handle<void*> ha(pFroms, myFree);

		LPTSTR pTo = CreateDNString(lpFileTo, pnRet);
		stlsoft::scoped_handle<void*> hi(pTo, myFree);

		return SHCopyOrMoveFileImpl(cm, false, pTo, pFroms, pnRet);
	}
	BOOL SHMoveFile(LPCTSTR lpFileTo, const std::vector<std::wstring>& sourcefiles, int* pnRet)
	{
		return SHCopyOrMoveFile(CopyORMove_Move, lpFileTo, sourcefiles, pnRet);
	}
	// Copy many files to 1 folder
	BOOL SHCopyFile(LPCTSTR lpFileTo, const std::vector<std::wstring>& sourcefiles, int* pnRet)
	{
		return SHCopyOrMoveFile(CopyORMove_Copy, lpFileTo, sourcefiles, pnRet);
	}


	// move multiple files to multiple files
	BOOL SHCopyOrMoveFile(CopyORMove cm, const vector<wstring>& destfiles, const vector<wstring>& sourcefiles, int* pnRet)
	{
		LPTSTR pFroms = CreateDNString(sourcefiles, pnRet);
		stlsoft::scoped_handle<void*> ha(pFroms, myFree);

		LPTSTR pTos = CreateDNString(destfiles, pnRet);
		stlsoft::scoped_handle<void*> hi(pTos, myFree);
		
		return SHCopyOrMoveFileImpl(cm, true, pTos, pFroms, pnRet);
	}
	BOOL SHMoveFile(const vector<wstring>& destfiles, const vector<wstring>& sourcefiles, int* pnRet)
	{
		return SHCopyOrMoveFile(CopyORMove_Move, destfiles, sourcefiles, pnRet);
	}
	BOOL SHCopyFile(const vector<wstring>& destfiles, const vector<wstring>& sourcefiles, int* pnRet)
	{
		return SHCopyOrMoveFile(CopyORMove_Copy, destfiles, sourcefiles, pnRet);
	}

} // namespace