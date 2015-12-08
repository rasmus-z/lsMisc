#pragma once

#include <windows.h>
#include <tchar.h>
#include <vector>

#include "tstring.h"

namespace Ambiesoft {

//enum ArgType {
//	ArgType_Int,
//	ArgType_Uint,
//	ArgType_String,
//	ArgType_UrlEncodedUtf8String,
//};


struct CAvailableCommandLineInfo
{
	int nID_;
	LPCTSTR pOption_;
//	ArgType argtype_;
	unsigned long argcountflag_;
};

struct CInputCommandLineInfo
{
	int nID_;
	tstring option_;
	tstring value_;
};

class CCommandLineParser
{
	int argc_;
	LPTSTR* targv_;

	static tstring GetToken(LPCTSTR p)
	{
		tstring ret;
		if(!p || !p[0])
			return ret;

		bool dq=false;
		bool sq=false;

		for( ; *p ; ++p)
		{
			if(_istspace(*p))
			{
				if(!dq && !sq)
					break;
			}
			else if(*p == _T('"'))
			{
				if(!sq)
				{
					dq = true;
					continue;
				}
			}
			else if(*p == _T('\''))
			{
				if(!dq)
				{
					sq = true;
					continue;
				}
			}

			ret += *p;
		}
		return ret;
	}
			
	CAvailableCommandLineInfo* pCLI_;
	size_t clicount_;

	CAvailableCommandLineInfo* FindAvailableCL(LPCTSTR pOption)
	{
		for(size_t i=0 ; i < clicount_ ; ++i)
		{
			CAvailableCommandLineInfo* pCLI = pCLI_;
			pCLI += i;
			if(_tcscmp(pCLI->pOption_, pOption)==0)
			{
				return pCLI;
			}
		}
		return NULL;
	}

	std::vector<CInputCommandLineInfo> inputs_;
	std::vector<CInputCommandLineInfo>::iterator current_;
	std::vector<CInputCommandLineInfo>::iterator* pcurrent_;
public:
	CInputCommandLineInfo* GetNext()
	{
		if(!pcurrent_)
		{
			pcurrent_ = &current_;
			*pcurrent_ = inputs_.begin();
		}
		else
			++(*pcurrent_);

		if(*pcurrent_ == inputs_.end())
		{
			pcurrent_=NULL;
			return NULL;
		}

		return &**pcurrent_;
	}

	CCommandLineParser(int argc, LPTSTR* targv,CAvailableCommandLineInfo* pCLI, size_t count)
	{
		pcurrent_ = NULL;

		argc_ = argc;
		targv_ = targv;
		pCLI_ = pCLI;
		clicount_ = count;

		for(int i=1 ; i < argc ; ++i)
		{
			LPCTSTR pArgv = targv[i];
			if(!pArgv || !pArgv[0])
				continue;

			if(pArgv[0]==_T('-') || pArgv[0]==_T('/'))
			{
				CAvailableCommandLineInfo* pCLI = FindAvailableCL(pArgv);	
				if(!pCLI)
				{
					CInputCommandLineInfo icli;
					icli.nID_ = -1;
					icli.option_ = pArgv;
					inputs_.push_back(icli);
					continue;
				}

				if(pCLI->argcountflag_ & 0x1)
				{
					++i;
					if( i >= argc )
					{
						CInputCommandLineInfo icli;
						icli.nID_ = pCLI->nID_;
						icli.option_ = pArgv;
						inputs_.push_back(icli);
						break;
					}
					
					LPCTSTR pArgv2 = targv[i];
					CInputCommandLineInfo icli;
					icli.nID_ = pCLI->nID_;
					icli.option_ = pArgv;
					icli.value_ = pArgv2;
					inputs_.push_back(icli);
					continue;
				}
			}
			else
			{
				CAvailableCommandLineInfo* pCLI = FindAvailableCL(_T(""));	
				if(!pCLI)
				{
					CInputCommandLineInfo icli;
					icli.nID_ = -2;
					icli.option_ = _T("");
					icli.value_ = pArgv;
					inputs_.push_back(icli);
					continue;
				}
				else
				{
					CInputCommandLineInfo icli;
					icli.nID_ = pCLI->nID_;
					icli.option_ = _T("");
					icli.value_ = pArgv;
					inputs_.push_back(icli);
					continue;

				}
			}
		}
	}
};


} // namespace