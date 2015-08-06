#pragma warning(disable:4786)

#include <windows.h>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdarg>
#include <cassert>
#include <stdio.h>

using namespace std;
#include "stdwin32.h"


namespace stdwin32 {

static const wchar_t* Nil=L"";

wstring stdGetModuleFileNameW(HINSTANCE hInst)
{
	LPWSTR p = NULL;
	size_t size = 64;
	for(;;)
	{
		p = (LPWSTR)realloc(p, size*sizeof(wchar_t));
		if(GetModuleFileNameW(hInst, p, size) < size)
			break;
		size*=2;
	}
	
	wstring ret=p;
	free((void*)p);
	return ret;
}

BOOL stdIsFullPath(LPCWSTR pD)
{
	if(!pD || pD[0]==0)
		return FALSE;

	if(pD[0]==L'/')
		return TRUE;

	if( !((L'a' <= pD[0] && pD[0] <+ L'z') ||
		  (L'A' <= pD[0] && pD[0] <+ L'Z')) )
	{
		return FALSE;
	}

	if(pD[1]==L':')
		return TRUE;

	return FALSE;
}


wstring stdCombinePath(LPCWSTR pD1, LPCWSTR pD2)
{
	
	if(!pD1 || !pD1[0])
		return pD2;

	if(!pD2 || !pD2[0] )
		return pD1;

	if(stdIsFullPath(pD2))
		return pD2;

	wstring ret = pD1;
	wstring::iterator it = ret.end();
	--it;
	if(*it!=L'\\')
		ret += L'\\';

	ret += pD2;
	return ret;
}


wstring stdGetParentDirectory(LPCWSTR pPath)
{
	if(!pPath || pPath[0]==0)
		return Nil;

	LPWSTR p = wcsdup(pPath);

	size_t len = wcslen(p);
	if(p[len-1]==L'\\')
		p[len-1]=0;

	LPWSTR pT = wcsrchr(p, L'\\');
	if(!pT)
	{
		free(p);
		return Nil;
	}

	*pT=0;
	wstring ret(p);
	free(p);
	return ret;
}


vector<wstring> stdSplitSCedPath(LPCWSTR pPath)
{
	vector<wstring> ret;

	LPCWSTR p = pPath;
	wstring cur;
	bool inq=false;
	for( ; *p ; ++p)
	{
		if(inq)
		{
			if(*p=='\"')
			{
				if(!cur.empty())
				{
					ret.push_back(cur);
					cur=L"";
				}
				inq=false;
			}
			else 
			{
				cur += *p;
			}
		}
		else
		{ // not inq
			if(*p=='\"')
			{
				inq=true;
			}
			else if(*p==L';')
			{
				if(!cur.empty())
				{
					ret.push_back(cur);
					cur=L"";
				}
			}
			else
			{
				cur+=*p;
			}
		}
	}

	if(!cur.empty())
	{
		ret.push_back(cur);
		cur=L"";
	}

	return ret;
}


std::string string_formatA(const std::string fmt, ...)
{
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::string str;
    va_list ap;
    while (1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, fmt);
        int n = _vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
		
        va_end(ap);
        if (n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
}
std::wstring string_formatW(const std::wstring fmt, ...)
{
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::wstring str;
    va_list ap;
    while (1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, fmt);
        int n = _vsnwprintf((WCHAR *)str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
}








} // namespace stdwin32