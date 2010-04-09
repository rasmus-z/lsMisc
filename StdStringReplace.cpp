////////////////////////////////////////////////////////////////////
//http://www20.big.or.jp/~koma/dir_WEB/contents/program/012.html
//文字列置換
//関数名	：　Replace()
//引数		：　string&型　src　置換対象の文字列オブジェクト
//		：　const char*型　pszKey 置換対象単語
//		：　const char*型　pszRepWord 置換後単語
//戻り値	：　なし
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

void StdStringReplaceA(std::string& src, LPCSTR pszKey, LPCSTR pszRepWord)
{
	int nPos = 0;
	size_t len = strlen(pszKey);
	while((nPos = src.find(pszKey, nPos)) != std::string::npos){
		src.replace(nPos, len, pszRepWord);
	}
	
	return;
}
