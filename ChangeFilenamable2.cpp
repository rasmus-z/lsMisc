#include <windows.h>
#include <tchar.h>
#include <string>



#include "tstring.h"
#include "ChangeFilenamable2.h"

using namespace std;

tstring ChangeFilenamable2(LPCTSTR pch)
{
	tstring ret;
	while( *pch != '\0' )
	{
		switch(*pch)
		{
		case _T('\\'):
			ret += _T("Åè");
			break;

		case _T(':'):
			ret += _T("ÅF");
			break;
			
		case _T('*'):
			ret += _T("Åñ");
			break;
			
		case _T('?'):
			ret += _T("ÅH");
			break;
			
		case _T('\"'):
			ret += _T("Åg");
			break;
			
		case _T('<'):
			ret += _T("ÅÉ");
			break;

		case _T('>'):
			ret += _T("ÅÑ");
			break;

		case _T('|'):
			ret += _T("Åb");
			break;

		case _T('/'):
			ret += _T("Å^");
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
					TCHAR szT[2];
					szT[0] = *pch;
					szT[1] = 0;
					ret += szT;
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