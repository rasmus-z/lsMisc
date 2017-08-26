#include "stdafx.h"
#include <string>

using namespace std;

static LPCWSTR SkipSpace(LPCWSTR p)
{
	for( ; *p ; ++p)
	{
		if(*p==L'\t' || *p==L' ' || *p==L'\r' || *p==L'\n')
		{
			// skip
		}
		else
		{
			break;
		}
	}
	return p;
}

std::wstring ReplaceFirstArg(LPCWSTR pCommandLine, LPCWSTR pReplace)
{
	if(!pCommandLine || pCommandLine[0]==0)
	{
		return pReplace;
	}
	
	LPCWSTR pFirstEndPos=NULL;
	TCHAR cSep=0;
	
	for(LPCWSTR p = SkipSpace(pCommandLine) ; *p ; ++p)
	{
		if(cSep)
		{
			// in the quote
		}
		else
		{
			pFirstEndPos = p;
			break;
		}

		if(*p==L'"' || *p==L'\'')
		{
			if(cSep==*p)
			{
				// quote ends
				cSep=0;
			}
			else 
			{
				// quote starts
				cSep=*p;
			}
		}
	}
	wstring ret;
	return ret;
}