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

#include <memory>
#include <algorithm>
#include <list>
#include <vector>
#include <locale>
#include <codecvt>
#include <unistd.h>

#include "stdosd.h"

#include "PathUtil.h"

using namespace std;




namespace Ambiesoft { namespace stdosd {

static wstring myPathCanonicalize(const wchar_t* pPath)
{
    vector<wstring> v = stdSplitString(wstring(pPath), wstring(L"/"));

    list<wstring> retV;
    for(size_t i=0 ; i < v.size(); ++i)
    {
        if(v[i]==L"..")
        {
            if(!retV.empty())
                retV.pop_front();
            continue;
        }
        else if(v[i]==L".")
        {
            continue;
        }
        retV.push_back(v[i]);
    }

    wstring ret;
    for(wstring& s : retV)
    {
        ret += s;
        ret += L"/";
    }
    return ret;
}
    static wstring toWstring(const string& s)
    {
        static_assert(sizeof(wchar_t)==sizeof(char16_t),"Wrong size");
        static_assert(sizeof(wchar_t)==4,"Wrong size");
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
        // std::string narrow = converter.to_bytes(wide_utf16_source_string);
        std::u16string restored = converter.from_bytes(s.c_str());
        //std::wstring wide = converter.from_bytes(s.data());
        return wstring(restored.c_str());
    }

    //server/			should be treated as root
	//server/folder		should not be treated as root
	//server/folder/aaa	should not be treated as root
    bool IsPathRoot(const wchar_t* pPath)
	{
        if (pPath == nullptr || pPath[0] == 0)
            return false;

        wstring path;
        if(pPath[0] != L'/')
        {
            // not fullpath
            unique_ptr<char> cur(get_current_dir_name());
            wstring wcur = toWstring(string(cur.get()));
            if(wcur.empty() || wcur[wcur.length()-1] != L'/')
                wcur += L"/";
            wcur += pPath;
        }
        else
        {
            path = pPath;
        }
        wstring tmp = myPathCanonicalize(path.c_str());
        return tmp.empty() || tmp==L"/";
	}

    bool IsBothEmptyOrNULL(const wchar_t* p1, const wchar_t* p2)
	{
        if (p1 == nullptr || p1[0] == 0)
            return p2 == nullptr || p2[0] == 0;

        return false;
	}

	int myPathGetComponentCount(const wchar_t* p)
	{
        wstring s = p;
        auto n = std::count(s.begin(), s.end(), L'/');

        return static_cast<int>(n);
	}
    bool IsSamePathDepth(const wchar_t* p1org, const wchar_t* p2org)
	{
		return myPathGetComponentCount(p1org) == myPathGetComponentCount(p2org);
	}

    bool IsPathSame(const wchar_t* p1, const wchar_t* p2)
	{
        return wcscmp(p1, p2) == 0;
	}


    static wstring myPathCommonPrefix(const wchar_t* p1, const wchar_t* p2)
    {
        if(!p1 || !p1[0])
            return wstring();
        if(!p2 || !p2[0])
            return wstring();

        size_t len1 = wcslen(p1);
        size_t len2 = wcslen(p2);
        size_t minlen = std::min(len1,len2);
        size_t i;
        for(i=0 ; i < minlen; ++i)
        {
            if(p1[i] != p2[i])
                break;
        }

        wstring ret(p1);
        return ret.substr(0,i);
    }
    bool IsPathChildIncluded(const wchar_t* p1org, const wchar_t* p2org, std::wstring* pDupPath)
	{
		// http://d.hatena.ne.jp/s-kita/20101206/1291651401#PathCommonPrefix
//        wstring lpCommonPrefix;
//		int nCommonPrefixCharcters;

        wstring p1(p1org);
        wstring p2(p2org);

        if(!p1.empty() && p1[p1.length()-1]==L'/')
            p1.substr(0, p1.length()-1);
        if(!p2.empty() && p2[p2.length()-1]==L'/')
            p2.substr(0, p2.length()-1);

        wstring w1 = p1;
        wstring w2 = p2;


        wstring szT1;
        wstring szT2;

        // TODO: expand env values
//		if (ExpandEnvironmentStrings(w1.c_str(), szT1, _countof(szT1)) > _countof(szT1))
//			return FALSE;
//		if (ExpandEnvironmentStrings(w2.c_str(), szT2, _countof(szT2)) > _countof(szT2))
//			return FALSE;

		w1 = szT1;
		w2 = szT2;

        // TODO: canonicalize
        szT1 = myPathCanonicalize(w1.c_str());
        szT2 = myPathCanonicalize(w2.c_str());

        wstring dupPath;
        dupPath = myPathCommonPrefix(szT1.c_str(), szT2.c_str());


//		nCommonPrefixCharcters = PathCommonPrefix(szT1, szT2, lpCommonPrefix);
//		if (nCommonPrefixCharcters < 1)
//		{
//			return FALSE;
//		}

        if (!IsPathSame(szT1.c_str(), dupPath.c_str()) && !IsPathSame(szT2.c_str(), dupPath.c_str()))
		{
            return false;
		}

        if (IsSamePathDepth(szT1.c_str(), szT2.c_str()))
		{
            return false;
		}

        if (pDupPath)
            *pDupPath = dupPath;

        return true;
	}


	static bool HasDupPaths(
		const wstring& left, 
		const vector<wstring>& saPaths, 
		size_t startindex,
		size_t& hitindex,
		wstring& common)
	{
		for (size_t i = startindex; i < saPaths.size(); ++i)
		{
			wstring dupParent;
			if (IsPathChildIncluded(left.c_str(), saPaths[i].c_str(), &dupParent))
			{
				common = dupParent;
				hitindex = i;
				return true;
			}
		}
		return false;
	}

	bool checkDupPaths(const vector<wstring>& saPaths, 
		wstring& left,
		wstring& right,
		wstring& common)
	{
		for (size_t i = 0; i < saPaths.size(); ++i)
		{
			wstring tmpleft = saPaths[i];
			size_t hitindex = 0;
			if (HasDupPaths(tmpleft, saPaths, i + 1, hitindex, common))
			{
				left = tmpleft;
				right = saPaths[hitindex];
				return false;
			}
		}
		return true;
	}
}}
