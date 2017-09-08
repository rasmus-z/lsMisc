#pragma once

namespace Ambiesoft {
//void UrlEncode_SJIS_Obsolete(const unsigned char	*csource,
//						size_t	nSize,
//						char** pOut,
//						int bUseMalloc = 0);

char *UrlEncode(const unsigned char *pstr, size_t size = -1);
std::wstring UrlEncodeW(const wchar_t *pstr);






// utf8 retuns
unsigned char* UrlDecode(const char* penc, size_t* psize=NULL);


std::wstring UrlDecodeW(const char* penc);
std::wstring UrlDecodeW(const std::wstring& wenc);


} // namespace