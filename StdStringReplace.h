////////////////////////////////////////////////////////////////////
//http://www20.big.or.jp/~koma/dir_WEB/contents/program/012.html
//文字列置換
//関数名	：　Replace()
//引数		：　string&型　src　置換対象の文字列オブジェクト
//		：　const char*型　pszKey 置換対象単語
//		：　const char*型　pszRepWord 置換後単語
//戻り値	：　なし

#include "tstring.h"

void StdStringReplaceW(std::wstring& src, LPCWSTR pszKey, LPCWSTR pszRepWord);
void StdStringReplaceA(std::string& src, LPCSTR pszKey, LPCSTR pszRepWord);

#ifdef UNICODE
#else
#endif