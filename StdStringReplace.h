

#pragma once


std::wstring StdStringReplaceW(std::wstring str, const std::wstring& from, const std::wstring& to);
std::string  StdStringReplaceA(std::string str,  const std::string& from,  const std::string& to);

#ifdef UNICODE
	#define StdStringReplace StdStringReplaceW
#else
	#define StdStringReplace StdStringReplaceA
#endif