////////////////////////////////////////////////////////////////////
//http://www20.big.or.jp/~koma/dir_WEB/contents/program/012.html

#include <windows.h>
#include "StdStringReplace.h"

void StdStringReplaceW(std::wstring& src, LPCWSTR pszKey, LPCWSTR pszRepWord)
{
	int nPos = 0;
	size_t len = wcslen(pszKey);
	while((nPos = src.find(pszKey, nPos)) != std::string::npos){
		src.replace(nPos, len, pszRepWord);
	}
	
	return;
}

void StdStringReplaceA_obsolete(std::string& src, LPCSTR pszKey, LPCSTR pszRepWord)
{
	int nPos = 0;
	size_t len = strlen(pszKey);
	while((nPos = src.find(pszKey, nPos)) != std::string::npos){
		src.replace(nPos, len, pszRepWord);
	}
	
	return;
}
