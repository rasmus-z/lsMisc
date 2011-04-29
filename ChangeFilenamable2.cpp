#include <windows.h>
#include <tchar.h>
#include <string>
#include "ChangeFilenamable2.h"
template<class E, class T, class A>

static std::basic_string<E,T,A>
replace_all(
  const std::basic_string<E,T,A>& source,   // source’†‚É‚ ‚é
  const std::basic_string<E,T,A>& pattern,  // pattern‚ğ
  const std::basic_string<E,T,A>& placement // placement‚É’u‚«Š·‚¦‚é
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
		std::string(_T(""))
		);
	ret=replace_all(
		ret,
		std::string(_T(":")),
		std::string(_T("F"))
		);
	ret=replace_all(
		ret,
		std::string(_T("*")),
		std::string(_T("–"))
		);
	ret=replace_all(
		ret,
		std::string(_T("?")),
		std::string(_T("H"))
		);
	ret=replace_all(
		ret,
		std::string(_T("\"")),
		std::string(_T("h"))
		);
	ret=replace_all(
		ret,
		std::string(_T("<")),
		std::string(_T("ƒ"))
		);
	ret=replace_all(
		ret,
		std::string(_T(">")),
		std::string(_T("„"))
		);
	ret=replace_all(
		ret,
		std::string(_T("|")),
		std::string(_T("b"))
		);
	ret=replace_all(
		ret,
		std::string(_T("/")),
		std::string(_T("^"))
		);




	return ret;
}