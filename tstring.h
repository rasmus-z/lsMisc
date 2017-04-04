#pragma once


#include <string>
#include <iostream>

#ifdef UNICODE
	typedef std::wstring tstring;
	typedef std::wifstream tifstream;
	typedef std::wofstream tofstream;
	inline std::wostream& tcoutfunc() { return std::wcout; }
	#define tcout tcoutfunc()
#else
	typedef std::string tstring;
	typedef std::ifstream tifstream;
	typedef std::ofstream tofstream;
	inline std::ostream& tcoutfunc() { return std::cout; }
	#define tcout tcoutfunc()
#endif
