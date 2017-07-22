#pragma once


#ifdef UNICODE
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif


namespace Ambiesoft {


class COption
{
	std::vector<tstring> options_;

	unsigned long argcountflag_;
	std::vector<tstring> values_;
	bool hadOption_;
	void AddValue(const tstring& value);
	void setHadOption();
	bool isMatchOption(const tstring& option) const;
	tstring getFirstOption() const;
public:
	COption(tstring option, unsigned long acf);
	COption(tstring option);

	friend class CCommandLineParser;
	
	bool hadOption() const;
	bool hadValue() const;

	tstring getValueStrings() const;
	tstring getFirstValue() const;
	size_t getValueCount() const;
};


class CCommandLineParser
{
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
			if( (*it)->isMatchOption(option) )
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
			ret += it->getFirstOption();
			if(!it->hadValue())
			{
				ret += _T(" ");
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
#ifdef UNICODE
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