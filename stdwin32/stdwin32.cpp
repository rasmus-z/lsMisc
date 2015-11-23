#pragma warning(disable:4786)
#include "stdafx.h"
#pragma warning(disable:4786)


#include <windows.h>

#include <algorithm> 
#include <cassert>
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <functional> 
#include <locale>
#include <string>
#include <vector>







using namespace std;
#include "stdwin32.h"


namespace stdwin32 {

static const wchar_t* Nil=L"";

string stdGetModuleFileNameA(HINSTANCE hInst)
{
	LPSTR p = NULL;
	size_t size = 64;
	for(;;)
	{
		p = (LPSTR)realloc(p, size*sizeof(char));
		if(GetModuleFileNameA(hInst, p, size) < size)
			break;
		size*=2;
	}
	
	string ret=p;
	free((void*)p);
	return ret;
}
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

wstring stdCombinePath(const wstring& d1, const wstring& d2)
{
	return stdCombinePath(d1.c_str(), d2.c_str());
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


wstring stdGetParentDirectory(LPCWSTR pPath, bool bAddBackslach)
{
	if(!pPath || pPath[0]==0)
		return Nil;

	LPWSTR p = _wcsdup(pPath);

	size_t len = wcslen(p);
	if(p[len-1]==L'\\')
		p[len-1]=0;

	LPWSTR pT = wcsrchr(p, L'\\');
	if(!pT)
	{
		free(p);
		return Nil;
	}

	if(!bAddBackslach)
		*pT=0;
	else
		*(pT+1)=0;

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




std::string trimA(const std::string& str,
                 const std::string& whitespace)
{
	const std::string::size_type strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const std::string::size_type strEnd = str.find_last_not_of(whitespace);
    const std::string::size_type strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}
std::wstring trimW(const std::wstring& str,
                 const std::wstring& whitespace)
{
    const std::wstring::size_type strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return L""; // no content

    const std::wstring::size_type strEnd = str.find_last_not_of(whitespace);
    const std::wstring::size_type strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}





bool isTdigit(const tstring& str)
{
	if(str.size()==0)
		return false;

	for(tstring::const_iterator it=str.begin() ; it != str.end() ; ++it)
	{
		if(!isdigit(*it))
			return false;
	}
	return true;
}


wstring stdGetCurrentDirectory()
{
	DWORD len = GetCurrentDirectory(0, NULL);
	TCHAR* p = new TCHAR[len];
	p[0]=0;
	GetCurrentDirectory(len,p);
	wstring ret(p);
	delete[] p;
	return ret;
}





} // namespace stdwin32