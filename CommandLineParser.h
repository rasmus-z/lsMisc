#pragma once

namespace Ambiesoft {

enum ArgType {
	ArgType_Int,
	ArgType_Uint,
	ArgType_String,
	ArgType_UrlEncodedUtf8String,
};


struct CCommandLineInfo
{
	int nID_;
	LPCSTR pOption_;
	ArgType argtype_;
	unsigned long argcountflag_;
};

class CCommandLineParser
{
public:
	bool construct(CCommandLineInfo* pCLI, size_t count)
	{
		return true;
	}
};


} // namespace