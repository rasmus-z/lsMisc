//#pragma once
#ifdef _WIN32
#ifdef UNICODE
#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#endif
#endif

#include <vector>
#include <string>
#include <algorithm>



namespace Ambiesoft {

	enum ArgCount
	{
		ArgCount_Zero = 0,
		ArgCount_One = 1,
		ArgCount_Two = 2,
		ArgCount_OneORTwo = 3,
		ArgCount_Three = 4,
		ArgCount_OneORThree = 5,
		ArgCount_TwoORThree = 6,
		ArgCount_OneORTwoORThree = 7,
		ArgCount_Four = 8,

		ArgCount_Infinite = 0xffffffff,
	};
	template <class myStringType> class BasicCommandLineParser;
	
	template<class myStringType> class BasicOption
	{
		std::vector<myStringType> options_;

		ArgCount argcountflag_;
		std::vector<myStringType> values_;
		bool hadOption_;
		void AddValue(const myStringType& value)
		{
			setHadOption();
			values_.push_back(value);

		}
		void setHadOption()
		{
			hadOption_ = true;
		}
		bool isMatchOption(const myStringType& option) const
		{
			std::vector< myStringType >::const_iterator cIter = find(options_.begin(), options_.end(), option);
			return cIter != options_.end();
		}
		myStringType getFirstOption() const
		{
			return options_[0];
		}

		void setArgFlag(int exactcount)
		{
			if (exactcount <= 0)
				argcountflag_ = ArgCount_Zero;
			else
			{
				--exactcount;
				argcountflag_ = (ArgCount)(1 << exactcount);
			}
		}
	public:
		BasicOption()
		{
			options_.push_back(_T(""));
			argcountflag_ = ArgCount_Infinite;
		}
		BasicOption(myStringType option, ArgCount acf)
		{
			options_.push_back(option);
			argcountflag_ = acf;
		}
		BasicOption(myStringType option, const int exactcount)
		{
			options_.push_back(option);
			setArgFlag(exactcount);
		}
		BasicOption(myStringType option1, myStringType option2, ArgCount acf)
		{
			options_.push_back(option1);
			options_.push_back(option2);
			argcountflag_ = acf;
		}
		BasicOption(myStringType option1, myStringType option2, const int exactcount)
		{
			options_.push_back(option1);
			options_.push_back(option2);

			setArgFlag(exactcount);
		}
		BasicOption(myStringType option)
		{
			options_.push_back(option);
			argcountflag_ = ArgCount_Zero;

		}
		BasicOption(myStringType option1, myStringType option2)
		{
			options_.push_back(option1);
			options_.push_back(option2);
		}

		friend class BasicCommandLineParser<myStringType>;

		bool hadOption() const
		{
			return hadOption_;
		}
		bool hadValue() const
		{
			return !values_.empty();
		}

		myStringType getValueStrings() const
		{
			myStringType ret;
			bool looped = false;
			for (std::vector<myStringType>::const_iterator it = values_.begin(); it != values_.end(); ++it)
			{
				if (looped)
				{
					ret += _T(" ");
				}
				looped = true;
				ret += *it;
			}
			return ret;
		}
		myStringType getFirstValue() const
		{
			myStringType ret;
			if (values_.empty())
				return ret;
			return values_[0];
		}
		unsigned int getFirstValueAsUInt() const
		{
			return _ttoi(getFirstValue().c_str());
		}
		unsigned long long getFirstValueAsUInt64() const
		{
			return _ttoi64(getFirstValue().c_str());
		}
		void* getFirstValueAsPointer() const
		{
			if (sizeof(void*) == sizeof(unsigned int))
				return (void*)getFirstValueAsUInt();
			else
				return (void*)getFirstValueAsUInt64();
		}
		myStringType getValue(int index)
		{
			return values_[index];
		}
		size_t getValueCount() const
		{
			return values_.size();
		}
	};


#ifdef UNICODE
	typedef BasicOption<std::wstring> COption;
#else
	typedef BasicOption<std::string> COption;
#endif


	



	template <class myStringType> class BasicCommandLineParser
	{
		static myStringType GetToken(LPCWSTR p)
		{
			myStringType ret;
			if (!p || !p[0])
				return ret;

			bool dq = false;
			bool sq = false;

			for (; *p; ++p)
			{
				if (_istspace(*p))
				{
					if (!dq && !sq)
						break;
				}
				else if (*p == _T('"'))
				{
					if (!sq)
					{
						dq = true;
						continue;
					}
				}
				else if (*p == _T('\''))
				{
					if (!dq)
					{
						sq = true;
						continue;
					}
				}

				ret += *p;
			}
			return ret;
		}

		
		typedef std::vector<BasicOption<myStringType>*> POPTIONARRAY;
		typedef std::vector<BasicOption<myStringType> > OPTIONARRAY;

		typedef BasicOption<myStringType> myOptionType;
		POPTIONARRAY availables_;
		OPTIONARRAY unknowns_;
		bool empty_;

		myOptionType* FindAvailableCL(const myStringType& option)
		{
			for (POPTIONARRAY::iterator it = availables_.begin(); it != availables_.end(); ++it)
			{
				if ((*it)->isMatchOption(option))
				{
					return *it;
				}
			}
			return NULL;
		}


	public:
		BasicCommandLineParser()
		{
			empty_ = true;
		}
		bool isEmpty() const
		{
			return empty_;
		}
		myStringType getUnknowOptionStrings() const
		{
			myStringType ret;
			for (OPTIONARRAY::const_iterator it = unknowns_.begin(); it != unknowns_.end(); ++it)
			{
				ret += it->getFirstOption();
				if (!it->hadValue())
				{
					ret += _T(" ");
					ret += it->getValueStrings();
				}
			}
			return ret;
		}
		bool hadUnknownOption() const
		{
			return !unknowns_.empty();
		}
		void AddOption(COption* cli)
		{
			availables_.push_back(cli);
		}
#ifdef _WIN32
#ifdef UNICODE
		void Parse()
		{
			LPWSTR *szArglist;
			int nArgs;

			szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
			if (NULL == szArglist)
			{
				return;
			}
			Parse(nArgs, szArglist);
			LocalFree(szArglist);
		}
#endif
#endif
		void Parse(LPCTSTR pArg)
		{
			for(LPCTSTR p = pArg; *p ; p=_tcsinc(p))
			{
			
			}
		}

		void Parse(int argc, LPTSTR* targv)
		{
			for (int i = 1; i < argc; ++i)
			{
				LPCTSTR pArgv = targv[i];
				if (!pArgv || !pArgv[0])
					continue;

				empty_ = false;

				if (pArgv[0] == _T('-') || pArgv[0] == _T('/'))
				{
					COption* pA = FindAvailableCL(pArgv);
					if (!pA)
					{
						//CInputCommandLineInfo icli;
						//icli.nID_ = -1;
						//icli.option_ = pArgv;
						unknowns_.push_back(COption(pArgv));
						continue;
					}

					pA->setHadOption();

					if (pA->argcountflag_ & 0x1)
					{
						++i;
						if (i >= argc)
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
					if (!pA)
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

			

#ifdef UNICODE
	typedef BasicCommandLineParser<std::wstring> CCommandLineParser;
#else
	typedef BasicCommandLineParser<std::string> CCommandLineParser;
#endif

	
} // namespace