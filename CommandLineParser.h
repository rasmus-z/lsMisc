//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#pragma once
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


	inline static bool StringCompare(const std::wstring& left, const std::wstring& right, bool ignoreCase=false)
	{
		if (ignoreCase)
			return _wcsicmp(left.c_str(), right.c_str()) == 0;
		else
			return wcscmp(left.c_str(), right.c_str()) == 0;
	}


	std::wstring Utf8UrlDecode(const std::wstring& ws);

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


	// 000000001(1) = only zero
	// 000000010(2) = only one
	// 000000011(3) = zero or one
	// 000000100(4) = only two
	// 000000101(5) = zero or two
	// 000000110(6) = one or two
	// 000000111(7) = zero, one or two
	// 000001000(8) = only tree
	enum ArgCount
	{
		ArgCount_Uninitialized = 0,
		ArgCount_Zero = 1,
		ArgCount_One = 2,
		ArgCount_ZeroOrOne = 3,
		ArgCount_Two = 4,
		ArgCount_ZeroOrTwo = 5,
		ArgCount_OneOrTwo = 6,
		ArgCount_ZeroOrOneOrTwo = 7,
		ArgCount_Three = 8,

		ArgCount_Infinite = 0xffffffff,
	};

	enum CaseFlags
	{
		CaseFlag_CaseDefault,
		CaseFlag_CaseSensitive,
		CaseFlag_CaseInsensitive,
	};
	enum ArgEncodingFlags
	{
		ArgEncoding_Default,
		ArgEncoding_UTF8UrlEncode,
	};

	template <class myStringType, class myOptionType> 
	class BasicCommandLineParser;
	
	template<class myStringType> 
	class BasicOption
	{
		typedef BasicOption<myStringType> MyT_;
		typedef typename myStringType::traits_type::char_type Elem;
		typedef myStringType MyS_;

		class UserTarget
		{
			bool* pBool_;
			int* pInt_;
			MyS_* pMys_;

			void init()
			{
				pBool_ = NULL;
				pInt_ = NULL;
				pMys_ = NULL;
			}
			UserTarget()
			{
				init();
			}
			
			void setBoolTarget(bool* pb)
			{
				assert(pBool_==NULL);
				pBool_=pb;
			}
			void setIntTarget(int* pi)
			{
				assert(pInt_ == NULL);
				pInt_ = pi;
			}
			void setMysTarget(MyS_* pM)
			{
				assert(pMys_==NULL);
				pMys_=pM;
			}

			void setTrue()
			{
				if (pBool_)
					*pBool_ = true;
				if (pInt_)
					*pInt_ = 1;

			}
			void setMys(const MyS_& mys)
			{
				if(pBool_)
				{
					if(
						StringCompare(mys, L"0") ||
						StringCompare(mys,L"off",true) ||
						StringCompare(mys,L"false",true)
						)
					{
						*pBool_=false;
					}
					else
					{
						*pBool_=true;
					}
				}
				if(pInt_)
					*pInt_=AtoI(mys);
				if(pMys_)
					*pMys_=mys;
			}
			friend MyT_;
		};

		std::vector<myStringType> options_;

		ArgCount argcountflag_;
		std::vector<myStringType> values_;
		bool hadOption_;
		bool parsed_;
		CaseFlags case_;
		ArgEncodingFlags encoding_;
		UserTarget *pTarget_;
		void setTarget(bool* pT)
		{
			if (pTarget_ == NULL)
				pTarget_ = new UserTarget();
			pTarget_->setBoolTarget(pT);
		}
		void setTarget(int* pT)
		{
			if (pTarget_ == NULL)
				pTarget_ = new UserTarget();
			pTarget_->setIntTarget(pT);
		}
		void setTarget(MyS_* pT)
		{
			if(pTarget_ == NULL)
				pTarget_ = new UserTarget();
			pTarget_->setMysTarget(pT);
		}			

		void AddValue(const myStringType& value)
		{
			setHadOption();
			if(pTarget_)
			{
				pTarget_->setMys(encoding_==ArgEncoding_UTF8UrlEncode? Utf8UrlDecode(value):value);
			}
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
		bool isMatchOption(const myStringType& option, bool ignoreCase) const
		{
			if (!ignoreCase)
			{
				std::vector< myStringType >::const_iterator cIter = find(options_.begin(), options_.end(), option);
				return cIter != options_.end();
			}
			
			for (std::vector< myStringType >::const_iterator cIter = options_.begin();
				cIter != options_.end();
				++cIter)
			{
				if (StringCompare(*cIter, option,true))
					return true;
			}
			return false;
		}
		bool isMatchOption(const myStringType& option) const
		{
			return isMatchOption(option, case_==CaseFlag_CaseInsensitive);
		}

		myStringType getFirstOption() const
		{
			return options_[0];
		}

		void setArgFlag(int exactcount)
		{
			argcountflag_ = (ArgCount)(1 << exactcount);
		}

		void setParsed()
		{
			parsed_ = true;
		}

		void init()
		{
			hadOption_ = false;
			parsed_ = false;
			argcountflag_ = ArgCount_Uninitialized;
			case_=CaseFlag_CaseDefault;
			pTarget_ = NULL;
			encoding_=ArgEncoding_Default;
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

		template<class InputIterator>
		BasicOption(InputIterator first, InputIterator last, const int exactcount)
		{
			init();
			while (first != last)
			{
				options_.push_back(*first);
				++first;
			}

			setArgFlag(exactcount);
		}
		
		BasicOption(myStringType option1, myStringType option2, ArgCount acf)
		{
			init();
			options_.push_back(option1);
			options_.push_back(option2);
			argcountflag_ = acf;
		}
		template<>
		BasicOption(myStringType option1, myStringType option2, const int exactcount)
		{
			init();
			options_.push_back(option1);
			options_.push_back(option2);

			setArgFlag(exactcount);
		}
		BasicOption(const Elem* p1, const Elem* p2, const int exactcount)
		{
			init();
			options_.push_back(p1);
			options_.push_back(p2);

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
		typedef typename myStringType MyS_;
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

		typedef BasicOption<myStringType> MyO_;
		POPTIONARRAY useroptions_;
		OPTIONARRAY inneroptions_;
		OPTIONARRAY unknowns_;
		bool empty_;
		bool parsed_;
		CaseFlags case_;

		MyO_* FindOption(const myStringType& option)
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

		void init()
		{
			empty_=true;
			parsed_=false;
			case_=CaseFlag_CaseDefault;
		}
	public:
		BasicCommandLineParser()
		{
			init();
		}
		BasicCommandLineParser(CaseFlags kase)
		{
			init();
			case_=kase;
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
		void AddOption(MyO_* cli)
		{
			if(cli->case_==CaseFlag_CaseDefault)
				cli->case_=case_;

			assert(!parsed_);

			check(cli);

			useroptions_.push_back(cli);
		}

#ifdef _DEBUG
		void check(MyO_* cli)
		{
			for (POPTIONARRAY::const_iterator it = useroptions_.begin();
				it != useroptions_.end();
				++it)
			{
				// check whether same option is added.
				assert((*it) != cli);

				// check same option string is added
				for(std::vector< myStringType >::const_iterator cIter=cli->options_.begin();
					cIter != cli->options_.end();
					++cIter)
				{
					assert(!( (*it)->isMatchOption(cIter->c_str(), 
						(*it)->case_==CaseFlag_CaseInsensitive || cli->case_==CaseFlag_CaseInsensitive)) );
				}
			}

			for(OPTIONARRAY::const_iterator it = inneroptions_.begin();
				it != inneroptions_.end();
				++it)
			{
				for(std::vector< myStringType >::const_iterator cIter=cli->options_.begin();
					cIter != cli->options_.end();
					++cIter)
				{
					assert(!( it->isMatchOption(cIter->c_str(), 
						it->case_==CaseFlag_CaseInsensitive || cli->case_ == CaseFlag_CaseInsensitive)) );
				}
			}
		}
#else
		void check(MyO_* cli){}
#endif
		// bool target
		void AddOption(
			MyS_ optionString1,
			int exactCount,
			bool* pTarget)
		{
			MyS_* first = &optionString1;
			MyS_* last = first + 1;
			AddOption(first, last, exactCount, pTarget);
		}

		template<class InputIterator>
		void AddOption(
			InputIterator first,
			InputIterator last,
			int exactCount,
			bool* pTarget)
		{
			MyO_ option(first, last, exactCount);
			option.case_ = case_;
			check(&option);
			*pTarget = false;
			option.setTarget(pTarget);
			inneroptions_.push_back(option);
		}
		template<>
		void AddOption(
			MyS_ optionString1,
			MyS_ optionString2,
			int exactCount,
			bool* pTarget)
		{
			const MyS_ ops[] = { optionString1, optionString2 };
			AddOption(ops, ops + _countof(ops), exactCount, pTarget);
		}

		void AddOption(
			const Elem* p1,
			const Elem* p2,
			int exactCount,
			bool* pTarget)
		{
			const Elem* ops[] = { p1, p2 };
			AddOption(ops, ops + _countof(ops), exactCount, pTarget);
		}


		// int target
		void AddOption(
			MyS_ optionString1,
			int exactCount,
			int* pTarget)
		{
			MyS_* first = &optionString1;
			MyS_* last = first + 1;
			AddOption(first, last, exactCount, pTarget);
		}
		template<class InputIterator>
		void AddOption(
			InputIterator first,
			InputIterator last,
			int exactCount,
			int* pTarget)
		{
			MyO_ option(first, last, exactCount);
			option.case_ = case_;
			check(&option);
			*pTarget = false;
			option.setTarget(pTarget);
			inneroptions_.push_back(option);
		}
		template<>
		void AddOption(
			MyS_ optionString1,
			MyS_ optionString2,
			int exactCount,
			int* pTarget)
		{
			const MyS_ ops[] = { optionString1, optionString2 };
			AddOption(ops, ops + _countof(ops), exactCount, pTarget);
		}

		void AddOption(
			const Elem* p1,
			const Elem* p2,
			int exactCount,
			int* pTarget)
		{
			const Elem* ops[] = { p1, p2 };
			AddOption(ops, ops + _countof(ops), exactCount, pTarget);
		}


		// wstring target
		void AddOption(
			const MyS_& optionString1,
			int exactCount,
			MyS_* pTarget,
			ArgEncodingFlags arf=ArgEncoding_Default)
		{
			MyO_ option(optionString1, exactCount);
			option.case_=case_;
			option.encoding_=arf;
			check(&option);
			*pTarget = L"";
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

	private:
		int ParseParam(int i, int argc, LPWSTR* targv, MyO_* pMyO)
		{
			if (pMyO->argcountflag_ == ArgCount_Zero)
				return i;

			if (pMyO->argcountflag_ == ArgCount_One)
			{
				++i;
				if (i >= argc)
				{
					return i;
				}

				LPCTSTR pArgv = targv[i];
				pMyO->AddValue(pArgv);
				return i;
			}

			// not yet
			assert(false);
			return 1044410;
		}
	public:
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
				{ // option argument
					MyO_* pA = FindOption(pArgv);
					if (!pA)
					{
						unknowns_.push_back(MyO_(pArgv));
						continue;
					}

					pA->setHadOption();

					i = ParseParam(i, argc, targv, pA);
				}
				else 
				{ // value argument
					myOptionType* pA = FindOption((L""));
					if (!pA)
					{
						//CInputCommandLineInfo icli;
						//icli.nID_ = -2;
						//icli.option_ = _T("");
						//icli.value_ = pArgv;
						unknowns_.push_back(MyO_((pArgv)));
						continue;
					}
					else
					{
						pA->AddValue((pArgv));
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

			for (OPTIONARRAY::iterator it = unknowns_.begin();
				it != unknowns_.end();
				++it)
			{
				it->setParsed();
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