//#pragma once
#ifdef _WIN32
#ifdef UNICODE
#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#endif // UNICODE
#endif // _WIN32

#include <vector>
#include <string>
#include <algorithm>



namespace Ambiesoft {

	inline static unsigned int AtoI(const std::string& s)
	{
		return atoi(s.c_str());
	}
	inline static unsigned int AtoI(const std::wstring& s)
	{
		return _wtoi(s.c_str());
	}

	inline static unsigned long long AtoI64(const std::string& s)
	{
		return _atoi64(s.c_str());
	}
	inline static unsigned long long AtoI64(const std::wstring& s)
	{
		return _wtoi64(s.c_str());
	}

	inline std::string StoSTD(LPCSTR p)
	{
		return p;
	}
	inline std::wstring StoSTD(LPCWSTR p)
	{
		return p;
	}

	inline std::string WStringToString(const std::wstring& w)
	{
		std::string ret;
		if (w.empty())
			return ret;

		size_t len = (w.length() + 1) * sizeof(wchar_t);
		char* pT = (char*)malloc(len);
		size_t retutrnvalue;

		if (0 != wcstombs_s(&retutrnvalue,
			pT,
			len,
			w.c_str(),
			len))
		{
			free(pT);
			return ret;
		}
		ret = pT;
		free(pT);
		return ret;
	}
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
	
	template <class myStringType, class myOptionType> 
	class BasicCommandLineParser;
	
	template<class myStringType> 
	class BasicOption
	{
		std::vector<myStringType> options_;

		ArgCount argcountflag_;
		std::vector<myStringType> values_;
		bool hadOption_;
		bool parsed_;

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

		void setParsed()
		{
			parsed_ = true;
		}


	public:
		BasicOption() : 
			hadOption_(false), parsed_(false)
		{
			options_.push_back(myStringType());
			argcountflag_ = ArgCount_Infinite;
		}
		BasicOption(myStringType option, ArgCount acf) : 
			hadOption_(false), parsed_(false)
		{
			options_.push_back(option);
			argcountflag_ = acf;
		}
		BasicOption(myStringType option, const int exactcount) :
			hadOption_(false), parsed_(false)
		{
			options_.push_back(option);
			setArgFlag(exactcount);
		}
		BasicOption(myStringType option1, myStringType option2, ArgCount acf) : 
			hadOption_(false), parsed_(false)
		{
			options_.push_back(option1);
			options_.push_back(option2);
			argcountflag_ = acf;
		}
		BasicOption(myStringType option1, myStringType option2, const int exactcount) : 
			hadOption_(false), parsed_(false)
		{
			options_.push_back(option1);
			options_.push_back(option2);

			setArgFlag(exactcount);
		}
		BasicOption(myStringType option) : 
			hadOption_(false), parsed_(false)
		{
			options_.push_back(option);
			argcountflag_ = ArgCount_Zero;
		}
		BasicOption(myStringType option1, myStringType option2) :
			hadOption_(false), parsed_(false)
		{
			options_.push_back(option1);
			options_.push_back(option2);
		}

		friend class BasicCommandLineParser<myStringType, BasicOption<myStringType> >;


		// Command line had the option
		bool hadOption() const
		{
			assert(parsed_);
			return hadOption_;
		}

		// Command line had value of the option
		bool hadValue() const
		{
			assert(parsed_);
			return !values_.empty();
		}

		// Get all values of the option
		myStringType getValueStrings() const
		{
			assert(parsed_);

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

		// Get first value of the option
		myStringType getFirstValue() const
		{
			assert(parsed_);

			myStringType ret;
			if (values_.empty())
				return ret;
			return values_[0];
		}


		unsigned int getFirstValueAsUInt() const
		{
			assert(parsed_);
			return AtoI(getFirstValue());
		}
		unsigned long long getFirstValueAsUInt64() const
		{
			assert(parsed_);
			return AtoI64(getFirstValue());
		}
		void* getFirstValueAsPointer() const
		{
			assert(parsed_);
			if (sizeof(void*) == sizeof(unsigned int))
				return (void*)getFirstValueAsUInt();
			else
				return (void*)getFirstValueAsUInt64();
		}
		myStringType getValue(int index)
		{
			assert(parsed_);
			return values_[index];
		}
		size_t getValueCount() const
		{
			assert(parsed_);
			return values_.size();
		}
	};


typedef BasicOption<std::wstring> COptionW;
typedef BasicOption<std::string> COptionA;

#ifdef UNICODE
	typedef COptionW COption;
#else
	typedef COptionA COption
#endif


	



	template <class myStringType, class myOptionType> class BasicCommandLineParser
	{
		typedef typename myStringType::traits_type::char_type Elem;
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

		myStringType stdstringToMyString(const std::wstring& ws)
		{
			return ws;
		}
		myStringType stdstringToMyString(const std::string& ws)
		{
			return WStringToString(ws);
		}

		typedef std::vector<BasicOption<myStringType>*> POPTIONARRAY;
		typedef std::vector<BasicOption<myStringType> > OPTIONARRAY;

		typedef BasicOption<myStringType> myOptionType;
		POPTIONARRAY availables_;
		OPTIONARRAY unknowns_;
		bool empty_;
		bool parsed_;

		myOptionType* FindOption(const myStringType& option)
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
		BasicCommandLineParser() :
			empty_(true), parsed_(false)
		{
		}
		bool isEmpty() const
		{
			return empty_;
		}
		myStringType getUnknowOptionStrings() const
		{
			assert(parsed_);

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
			assert(parsed_);
			return !unknowns_.empty();
		}
		void AddOption(myOptionType* cli)
		{
			assert(!parsed_);
#ifdef _DEBUG
			for (POPTIONARRAY::const_iterator it = availables_.begin();
				it != availables_.end();
				++it)
			{
				// check whether same option is added.
				assert((*it) != cli);
			}
#endif
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
		//void Parse(LPCWSTR pArg)
		//{
		//	for(LPCTSTR p = pArg; *p ; p=_tcsinc(p))
		//	{
		//	
		//	}
		//}

	
		void Parse(int argc, LPWSTR* targv)
		{
			assert(!parsed_);  // already parsed
			parsed_ = true;

			for (int i = 1; i < argc; ++i)
			{
				LPCWSTR pArgv = targv[i];
				if (!pArgv || !pArgv[0])
					continue;

				empty_ = false;

				if (pArgv[0] == L'-' || pArgv[0] == L'/')
				{
					myOptionType* pA = FindOption(stdstringToMyString(pArgv));
					if (!pA)
					{
						//CInputCommandLineInfo icli;
						//icli.nID_ = -1;
						//icli.option_ = pArgv;
						unknowns_.push_back(myOptionType(stdstringToMyString(pArgv)));
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
						pA->AddValue(stdstringToMyString(pArgv2));
						continue;
					}
				}
				else
				{
					myOptionType* pA = FindOption(stdstringToMyString(L""));
					if (!pA)
					{
						//CInputCommandLineInfo icli;
						//icli.nID_ = -2;
						//icli.option_ = _T("");
						//icli.value_ = pArgv;
						unknowns_.push_back(myOptionType(stdstringToMyString(pArgv)));
						continue;
					}
					else
					{
						//CInputCommandLineInfo icli;
						//icli.nID_ = pA->nID_;
						//icli.option_ = _T("");
						pA->AddValue(stdstringToMyString(pArgv));
						// pA.push_back(icli);
						continue;

					}
				}
			}


			// mark all options as parsed.
			// when user call some functions which is not added to parser or before parse(),
			// assert() fails.
			for (POPTIONARRAY::const_iterator it = availables_.begin();
				it != availables_.end();
				++it)
			{
				(*it)->setParsed();
			}
		}
	};

			

typedef BasicCommandLineParser<std::wstring, COptionW> CCommandLineParserW;
typedef BasicCommandLineParser<std::string, COptionA> CCommandLineParserA;

#ifdef UNICODE
typedef CCommandLineParserW CCommandLineParser;
#else
typedef CCommandLineParserA CCommandLineParser;
#endif

	
} // namespace