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

#include <windows.h>
#include <tchar.h>
#include <string>



#include "tstring.h"
#include "ChangeFilenamable.h"



using namespace std;

static tstring convertChar(TCHAR t1, bool bToSjisZen)
{

}

tstring ChangeFilenamable(LPCTSTR pch, bool bToSjisZen)
{
	tstring ret;
	while( *pch != '\0' )
	{
		switch(*pch)
		{
		case _T('\\'):
			ret += bToSjisZen ? _T("Åè") : _T("_");
			break;

		case _T(':'):
			ret += bToSjisZen ? _T("ÅF") : _T("_");
			break;
			
		case _T('*'):
			ret += bToSjisZen ? _T("Åñ") : _T("_");
			break;
			
		case _T('?'):
			ret += bToSjisZen ? _T("ÅH") : _T("_");
			break;
			
		case _T('\"'):
			ret += bToSjisZen ? _T("Åg") : _T("_");
			break;
			
		case _T('<'):
			ret += bToSjisZen ? _T("ÅÉ") : _T("_");
			break;

		case _T('>'):
			ret += bToSjisZen ? _T("ÅÑ") : _T("_");
			break;

		case _T('|'):
			ret += bToSjisZen ? _T("Åb") : _T("_");
			break;

		case _T('/'):
			ret += bToSjisZen ? _T("Å^") : _T("_");
			break;

		default:
			{

#ifdef UNICODE
				TCHAR szT[2];
				szT[0] = *pch;
				szT[1] = 0;
				ret += szT;
#else
				if(IsDBCSLeadByte((BYTE)(*pch)))
				{
					TCHAR szT[3];
					szT[0] = *pch;
					szT[1] = *(pch+1);
					szT[2] = 0;
					ret += szT;
				}
				else
				{
					if( 0 <= *pch && *pch <= 0x1F)
					{
						// nothing
					}
					else
					{
						TCHAR szT[2];
						szT[0] = *pch;
						szT[1] = 0;
						ret += szT;
					}
				}
#endif
			}
			break;
		}
		pch = (TCHAR*)_tcsinc(pch);
	}
	
	return ret;
}








/*
template<class E, class T, class A>

static std::basic_string<E,T,A>
replace_all(
  const std::basic_string<E,T,A>& source,   // sourceíÜÇ…Ç†ÇÈ
  const std::basic_string<E,T,A>& pattern,  // patternÇ
  const std::basic_string<E,T,A>& placement // placementÇ…íuÇ´ä∑Ç¶ÇÈ
  ) {
  std::basic_string<E,T,A> result(source);
  for ( std::string::size_type pos = 0 ;
        std::string::npos != (pos = result.find(pattern,pos));
        pos += placement.size() )
    result.replace(pos, pattern.size(), placement);
  return result;
}

std::string ChangeFilenamable2(LPCTSTR pch)
{
	std::string ret=pch;
	ret=replace_all(
		ret,
		std::string(_T("\\")),
		std::string(_T("Åè"))
		);
	ret=replace_all(
		ret,
		std::string(_T(":")),
		std::string(_T("ÅF"))
		);
	ret=replace_all(
		ret,
		std::string(_T("*")),
		std::string(_T("Åñ"))
		);
	ret=replace_all(
		ret,
		std::string(_T("?")),
		std::string(_T("ÅH"))
		);
	ret=replace_all(
		ret,
		std::string(_T("\"")),
		std::string(_T("Åh"))
		);
	ret=replace_all(
		ret,
		std::string(_T("<")),
		std::string(_T("ÅÉ"))
		);
	ret=replace_all(
		ret,
		std::string(_T(">")),
		std::string(_T("ÅÑ"))
		);
	ret=replace_all(
		ret,
		std::string(_T("|")),
		std::string(_T("Åb"))
		);
	ret=replace_all(
		ret,
		std::string(_T("/")),
		std::string(_T("Å^"))
		);




	return ret;
}
*/