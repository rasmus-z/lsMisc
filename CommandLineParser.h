//#pragma once
#ifdef _WIN32
#ifdef UNICODE
#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#endif // UNICODE
#endif // _WIN32

#include <cassert>
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






	//// explicit_specialization.cpp  
	//template<class T> void f(T t)
	//{
	//};

	//// Explicit specialization of f with 'char' with the  
	//// template argument explicitly specified:  
	////  
	//template<> void f<char>(char c)
	//{
	//}

	//// Explicit specialization of f with 'double' with the  
	//// template argument deduced:  
	////  
	//template<> void f(double d)
	//{
	//}




	//inline std::string StoSTD(LPCSTR p)
	//{
	//	return p;
	//}
	//inline std::wstring StoSTD(LPCWSTR p)
	//{
	//	return p;
	//}

	//inline std::string WStringToString(const std::wstring& w)
	//{
	//	std::string ret;
	//	if (w.empty())
	//		return ret;

	//	size_t len = (w.length() + 1) * sizeof(wchar_t);
	//	char* pT = (char*)malloc(len);
	//	size_t retutrnvalue;

	//	if (0 != wcstombs_s(&retutrnvalue,
	//		pT,
	//		len,
	//		w.c_str(),
	//		len))
	//	{
	//		free(pT);
	//		return ret;
	//	}
	//	ret = pT;
	//	free(pT);
	//	return ret;
	//}
	//inline std::wstring StringToWString(const std::string& s)
	//{
	//	int nReqSize = MultiByteToWideChar(
	//		CP_UTF8,
	//		0,
	//		(const char*)s.c_str(),
	//		(int)-1,
	//		NULL,
	//		0);

	//	if ( nReqSize == 0 )
	//		return NULL;

	//	LPWSTR pOut = (LPWSTR)malloc(nReqSize*sizeof(WCHAR));
	//	int nRet = MultiByteToWideChar(
	//		CP_UTF8,
	//		0,
	//		(const char*)s.c_str(),
	//		(int)-1,
	//		pOut,
	//		nReqSize);

	//	if ( nRet==0 || nRet != nReqSize )
	//	{
	//		free(pOut);
	//		return NULL;
	//	}

	//	return pOut;
	//}

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
		typedef BasicOption<myStringType> MyT_;
		typedef myStringType MyS_;

		class UserTarget
		{
			bool* pBool_;
			int* pInt_;

			void init()
			{
				pBool_ = NULL;
				pInt_ = NULL;
			}
			UserTarget(bool* p)
			{
				init();
				pBool_ = p;
			}
			void setTrue()
			{
				if (pBool_)
					*pBool_ = true;
				if (pInt_)
					*pInt_ = 1;
			}
			friend MyT_;
		};

		std::vector<myStringType> options_;

		ArgCount argcountflag_;
		std::vector<myStringType> values_;
		bool hadOption_;
		bool parsed_;

		UserTarget *pTarget_;
		void setTarget(bool* pT)
		{
			assert(pTarget_ == NULL);
			pTarget_ = new UserTarget(pT);
		}
		void AddValue(const myStringType& value)
		{
			setHadOption();
			values_.push_back(value);
		}
		void setHadOption()
		{
			if(argcountflag_== ArgCount_Zero)
			{
				if (pTarget_)
				{
					pTarget_->setTrue();
				}
			}
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

		void init()
		{
			hadOption_ = false;
			parsed_ = false;
			pTarget_ = NULL;
		}
	public:
		BasicOption()
		{
			init();
			options_.push_back(myStringType());
			argcountflag_ = ArgCount_Infinite;
		}
		BasicOption(myStringType option, ArgCount acf) 
		{
			init();
			options_.push_back(option);
			argcountflag_ = acf;
		}
		BasicOption(myStringType option, const int exactcount) 
		{
			init();
			options_.push_back(option);
			setArgFlag(exactcount);
		}
		BasicOption(myStringType option1, myStringType option2, ArgCount acf) 
		{
			init();
			options_.push_back(option1);
			options_.push_back(option2);
			argcountflag_ = acf;
		}
		BasicOption(myStringType option1, myStringType option2, const int exactcount) 
		{
			init();
			options_.push_back(option1);
			options_.push_back(option2);

			setArgFlag(exactcount);
		}
		BasicOption(myStringType option) 
		{
			init();
			options_.push_back(option);
			argcountflag_ = ArgCount_Zero;
		}
		BasicOption(myStringType option1, myStringType option2)
		{
			init();
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


	



	template <class myStringType, class myOptionType> 
	class BasicCommandLineParser
	{
		typedef typename myStringType::traits_type::char_type Elem;
		typedef typename myOptionType::MyS_ MyOS_;

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
		POPTIONARRAY useroptions_;
		OPTIONARRAY inneroptions_;
		OPTIONARRAY unknowns_;
		bool empty_;
		bool parsed_;

		myOptionType* FindOption(const myStringType& option)
		{
			for (POPTIONARRAY::iterator it = useroptions_.begin(); it != useroptions_.end(); ++it)
			{
				if ((*it)->isMatchOption(option))
				{
					return *it;
				}
			}

			for (OPTIONARRAY::iterator it = inneroptions_.begin();
				it != inneroptions_.end();
				++it)
			{
				if (it->isMatchOption(option))
				{
					// operator * returns a reference
					return &(*it);
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
			useroptions_.push_back(cli);
		}
		void AddOption(
			const myStringType& optionString1,
			int exactCount,
			bool* pTarget)
		{
			myOptionType option(optionString1, exactCount);
			*pTarget = false;
			option.setTarget(pTarget);
			inneroptions_.push_back(option);
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
					myOptionType* pA = FindOption(pArgv);
					if (!pA)
					{
						//CInputCommandLineInfo icli;
						//icli.nID_ = -1;
						//icli.option_ = pArgv;
						unknowns_.push_back(myOptionType(pArgv));
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
						pA->AddValue((pArgv2));
						continue;
					}
				}
				else
				{
					myOptionType* pA = FindOption((L""));
					if (!pA)
					{
						//CInputCommandLineInfo icli;
						//icli.nID_ = -2;
						//icli.option_ = _T("");
						//icli.value_ = pArgv;
						unknowns_.push_back(myOptionType((pArgv)));
						continue;
					}
					else
					{
						//CInputCommandLineInfo icli;
						//icli.nID_ = pA->nID_;
						//icli.option_ = _T("");
						pA->AddValue((pArgv));
						// pA.push_back(icli);
						continue;

					}
				}
			}


			// mark all options as parsed.
			// when user call some functions which is not added to parser or before parse(),
			// assert() fails.
			for (POPTIONARRAY::const_iterator it = useroptions_.begin();
				it != useroptions_.end();
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