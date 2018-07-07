//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#include "stdafx.h"


#ifndef _WINDOWS_
#pragma message("including windows.h")
#include <windows.h>
#endif

#ifndef _INC_TCHAR
#pragma message("including tchar.h")
#include <tchar.h>
#endif

#ifndef _INC_SHLWAPI
#pragma message("including shlwlapi")
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#endif


#ifndef _STRING_
#pragma message("including string")
#include <string>
#endif

#ifndef _ALGORITHM_
#pragma message("including algorithm")
#include <algorithm>
#endif

#ifndef STLSOFT_INCL_STLSOFT_SMARTPTR_HPP_SCOPED_HANDLE
#pragma message("including scoped_handel.hpp")
#include <stlsoft/smartptr/scoped_handle.hpp>
#endif

#include "PathUtil.h"

using namespace std;


#pragma comment(lib,"shlwapi.lib")

namespace Ambiesoft {
	//server/			should be treated as root
	//server/folder		should not be treated as root
	//server/folder/aaa	should not be treated as root
	BOOL myPathIsRoot(LPCTSTR pPathorg)
	{
		if (!pPathorg || pPathorg[0] == 0)
			return FALSE;

		if (PathIsRelative(pPathorg))
		{
			TCHAR szT[MAX_PATH * 2];
			LPTSTR pT;
			if (GetFullPathName(pPathorg, _countof(szT), szT, &pT) > _countof(szT))
				return FALSE;
			return myPathIsRoot(szT);
		}
		LPWSTR pPath = NULL;
		LPWSTR pPathFree = NULL;
		BOOL ret = FALSE;
		do
		{
			if (pPathorg[wcslen(pPathorg) - 1] != L'\\')
			{
				size_t wordsize = (wcslen(pPathorg) + 1 + 1);
				size_t bytesize = wordsize * sizeof(WCHAR);

				pPathFree = pPath = (LPWSTR)malloc(bytesize);
				wcscpy_s(pPath, wordsize, pPathorg);

				PathAddBackslash(pPath);
			}
			else
			{
				pPath = (LPWSTR)pPathorg;
			}


			if (!PathIsUNC(pPath))
			{
				ret = PathIsRoot(pPath);
				break;
			}
			if (PathIsUNCServer(pPath))
			{
				ret = TRUE;
				break;
			}

			if (PathIsUNCServerShare(pPath))
			{
				ret = TRUE;
				break;
			}

			ret = FALSE;
		} while (false);

		free(pPathFree);
		return ret;
	}

	static BOOL IsBothEmptyOrNULL(LPCTSTR p1, LPCTSTR p2)
	{
		if (p1 == NULL || p1[0] == 0)
			return p2 == NULL || p2[0] == 0;
		return FALSE;
	}

	int myPathGetComponentCount(LPCTSTR p)
	{
		LPCTSTR pT = p;
		int i = 0;
		for (; pT = PathFindNextComponent(pT); ++i)
		{

		}
		return i;
	}
	BOOL myPathSameDepth(LPCTSTR p1org, LPCTSTR p2org)
	{
		return myPathGetComponentCount(p1org) == myPathGetComponentCount(p2org);
		//if (IsBothEmptyOrNULL(p1org, p2org))
		//	return TRUE;

		//LPTSTR p1 = _tcsdup(p1org);
		//LPTSTR p2 = _tcsdup(p2org);

		//PathMakePretty(p1);
		//PathMakePretty(p2);

		//PathRemoveBackslash(p1);
		//PathRemoveBackslash(p2);


		//return FALSE;
	}

	BOOL myPathIsSame(LPCTSTR p1, LPCTSTR p2)
	{
		return lstrcmpi(p1, p2) == 0;
	}
	BOOL myPathIsChildIncluded(LPCTSTR p1org, LPCTSTR p2org, std::wstring* pDupPath)
	{
		// http://d.hatena.ne.jp/s-kita/20101206/1291651401#PathCommonPrefix
		TCHAR lpCommonPrefix[MAX_PATH]; lpCommonPrefix[0] = 0;
		int nCommonPrefixCharcters;

		LPTSTR p1 = _tcsdup(p1org);
		stlsoft::scoped_handle<void*> ma1(p1, free);
		LPTSTR p2 = _tcsdup(p2org);
		stlsoft::scoped_handle<void*> ma2(p2, free);

		PathRemoveBackslash(p1);
		PathRemoveBackslash(p2);

		wstring w1 = p1;
		replace(w1.begin(), w1.end(), L'/', L'\\');
		wstring w2 = p2;
		replace(w2.begin(), w2.end(), L'/', L'\\');

		TCHAR szT1[MAX_PATH * 2];
		TCHAR szT2[MAX_PATH * 2];
		if (ExpandEnvironmentStrings(w1.c_str(), szT1, _countof(szT1)) > _countof(szT1))
			return FALSE;
		if (ExpandEnvironmentStrings(w2.c_str(), szT2, _countof(szT2)) > _countof(szT2))
			return FALSE;

		w1 = szT1;
		w2 = szT2;
		if (!PathCanonicalize(szT1, w1.c_str()))
			return FALSE;
		if (!PathCanonicalize(szT2, w2.c_str()))
			return FALSE;

		nCommonPrefixCharcters = PathCommonPrefix(szT1, szT2, lpCommonPrefix);
		if (nCommonPrefixCharcters < 1)
		{
			return FALSE;
		}

		//if (myPathIsRoot(lpCommonPrefix))
		//{
		//	return FALSE;
		//}

		if (!myPathIsSame(szT1, lpCommonPrefix) && !myPathIsSame(szT2, lpCommonPrefix))
		{
			return FALSE;
		}

		if (myPathSameDepth(szT1, szT2))
		{
			return FALSE;
		}

		if (pDupPath)
			*pDupPath = lpCommonPrefix;

		return TRUE;
	}


	static bool HasDupPaths(
		const wstring& left, 
		const vector<wstring>& saPaths, 
		int startindex, 
		int& hitindex, 
		wstring& common)
	{
		for (size_t i = startindex; i < saPaths.size(); ++i)
		{
			// CString tmp;
			wstring dupParent;
			if (myPathIsChildIncluded(left.c_str(), saPaths[i].c_str(), &dupParent))
			{
				common = dupParent.c_str();
				hitindex = i;
				return true;
			}
		}
		return false;
	}

	bool checkDupPaths(const vector<wstring>& saPaths, 
		wstring&left,
		wstring& right,
		wstring&common)
	{
		for (size_t i = 0; i < saPaths.size(); ++i)
		{
			wstring tmpleft = saPaths[i];
			int hitindex = 0;
			if (HasDupPaths(tmpleft, saPaths, i + 1, hitindex, common))
			{
				left = tmpleft;
				right = saPaths[hitindex];
				return false;
			}
		}
		return true;
	}
}