#pragma once

#include <windows.h>
#include <tchar.h>
#include <vector>
#include <string>

#ifdef UNICODE
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif

#ifdef __linux__ 
#elif _WIN32
	#include <Shellapi.h>
#else
#endif


namespace Ambiesoft {

//enum ArgType {
//	ArgType_Int,
//	ArgType_Uint,
//	ArgType_String,
//	ArgType_UrlEncodedUtf8String,
//};


class COption
{

	tstring option_;
//	ArgType argtype_;
	unsigned long argcountflag_;
	std::vector<tstring> values_;
	bool hadOption_;
	void AddValue(const tstring& value)
	{
		values_.push_back(value);
	}
	void setHadOption() {
		hadOption_=true;
	}
public:
	COption(tstring option, unsigned long acf) : hadOption_(false)
	{
		option_ = option;
		argcountflag_=acf;
	}
	COption(tstring option) : hadOption_(false)
	{
		option_ = option;
		argcountflag_=0;
	}
	friend class CCommandLineParser;
	
	bool hadOption() const {
		return hadOption_;
	}
	bool hadValue() const {
		return !values_.empty();
	}
	tstring getValueStrings() const {
		tstring ret;
		bool looped=false;
		for(std::vector<tstring>::const_iterator it=values_.begin() ; it != values_.end() ; ++it)
		{
			if(looped)
			{
				ret += L" ";
			}
			looped = true;
			ret += *it;
		}
		return ret;
	}
	tstring getFirstValue() const {
		tstring ret;
		if(values_.empty())
			return ret;
		return values_[0];
	}
};


class CCommandLineParser
{
	//int argc_;
	//LPTSTR* targv_;

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
			
	typedef std::vector<COption*> POPTIONARRAY;
	typedef std::vector<COption> OPTIONARRAY;
	POPTIONARRAY availables_;
	OPTIONARRAY unknowns_;
	bool empty_;

	COption* FindAvailableCL(const tstring& option)
	{
		for(POPTIONARRAY::iterator it=availables_.begin() ; it != availables_.end() ; ++it)
		{
			//COption* pCLI = pCLI_;
			//pCLI += i;
			// if(_tcscmp(pCLI->pOption_, pOption)==0)
			if( (*it)->option_ == option)
			{
				return *it;
			}
		}
		return NULL;
	}


public:
	CCommandLineParser()
	{
		empty_ = true;
	}
	bool isEmpty() const {
		return empty_;
	}
	tstring getUnknowOptionStrings() const {
		tstring ret;
		for(OPTIONARRAY::const_iterator it=unknowns_.begin() ; it != unknowns_.end() ; ++it)
		{
			ret += it->option_;
			if(!it->hadValue())
			{
				ret += L" ";
				ret += it->getValueStrings();
			}
		}
		return ret;
	}
	bool hadUnknownOption() const {
		return !unknowns_.empty();
	}
	void AddOption(COption* cli) {
		availables_.push_back(cli);
	}
#ifdef _WIN32
	void Parse()
	{
		LPWSTR *szArglist;
		int nArgs;

		szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
		if( NULL == szArglist )
		{
			return;
		}
		Parse(nArgs, szArglist);
		LocalFree(szArglist);
	}
#endif
	void Parse(int argc, LPTSTR* targv)
	{
		//argc_ = argc;
		//targv_ = targv;
		
		for(int i=1 ; i < argc ; ++i)
		{
			LPCTSTR pArgv = targv[i];
			if(!pArgv || !pArgv[0])
				continue;

			empty_ = false;

			if(pArgv[0]==_T('-') || pArgv[0]==_T('/'))
			{
				COption* pA = FindAvailableCL(pArgv);	
				if(!pA)
				{
					//CInputCommandLineInfo icli;
					//icli.nID_ = -1;
					//icli.option_ = pArgv;
					unknowns_.push_back(COption(pArgv));
					continue;
				}

				pA->setHadOption();

				if(pA->argcountflag_ & 0x1)
				{
					++i;
					if( i >= argc )
					{
						break;
					}
					
					LPCTSTR pArgv2 = targv[i];
					// pA->option_ = pArgv;
					pA->AddValue(pArgv2);
					continue;
				}
			}
			else
			{
				COption* pA = FindAvailableCL(_T(""));	
				if(!pA)
				{
					//CInputCommandLineInfo icli;
					//icli.nID_ = -2;
					//icli.option_ = _T("");
					//icli.value_ = pArgv;
					unknowns_.push_back(COption(pArgv));
					continue;
				}
				else
				{
					//CInputCommandLineInfo icli;
					//icli.nID_ = pA->nID_;
					//icli.option_ = _T("");
					pA->AddValue(pArgv);
					// pA.push_back(icli);
					continue;

				}
			}
		}
	}
};


} // namespace