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

#include <vector>
#include <string>
#include <cassert>

#ifndef _countof
#define _countof(t) (sizeof(t)/sizeof(t[0]))
#endif

namespace Ambiesoft {

	inline bool myIsSpace(char c)
	{
		return isspace(c) != 0;
	}
	inline bool myIsSpace(wchar_t c)
	{
		return iswspace(c) != 0;
	}

	inline const char* skipWS(const char* p)
	{
			while (*p && isspace(*p))
				++p;
			return p;
	}
	inline const wchar_t* skipWS(const wchar_t* p) 
	{
		while (*p && iswspace(*p))
			++p;
		return p;
	}

	inline void clearS(std::string& s)
	{
		s="";
	}
	inline void clearS(std::wstring& s)
	{
		s=L"";
	}

	const char* nextP(const char* p)
	{
		return CharNextA(p);
	}
	const wchar_t* nextP(const wchar_t* p)
	{
		++p;
		return p;
	}

	inline bool isDQ(char c) 
	{
		return c == '"';
	}
	inline bool isDQ(wchar_t c) 
	{
		return c == L'"';
	}

	template<class E>
	class CCommandLineStringBase
	{
		const E* p_;
		std::vector<size_t> offsets_;
		typedef std::vector<std::basic_string<E> > myVS;
		typedef char_traits<E> myTr;

		std::vector<std::basic_string<E> > args_;

	
//		E* getNext(E* p, bool& inQ)
//		{
//			if (!*p)
//			{
//				return p;
//			}
//
//			if (!inQ)
//				return skipWS(p);
//		}

//		bool loopOUTQ(E*& p)
//		{
//			++p;
//			if (isDQ(*p))
//			{
//				// start DQ
//				return true;
//			}
//
//			// continue outQ
//			return false;
//		}
//		bool loopINQ(E*& p)
//		{
//			if (isDQ(*p))
//			{
//				if (isDQ(*(p + 1)))
//				{
//					// doule DQ
//					p += 2;
//					return false; // continue INQ
//				}
//				else
//				{
//					// quote ends
//					p++;
//					return true;
//				}
//			}
//		}
	
		void init(const E* pCommandLine)
		{
			
			size_t clLen = myTr::length(pCommandLine);
			p_ = new E[clLen+1];
			myTr::copy((E*)p_, pCommandLine, clLen+1);

			if (!pCommandLine || !*pCommandLine)
				return;

			const E* p = skipWS(pCommandLine);
			if (!*p)
				return;

			bool inDQ = false;
			E c = 0;
			const E* pStart = p;
			//if ((pStart - p_) != 0)
			//{
			//	offsets_.push_back(0);
			//	args_.push_back(L"");
			//}
			//offsets_.push_back(pStart - pCommandLine);
			//args_.push_back(L"");

			std::basic_string<E> now;

			for (; *p; p=nextP(p))
			{
				if (!inDQ)
				{
					if (myIsSpace(*p))
					{
						// separator

						if (!now.empty())
						{
							offsets_.push_back(pStart - pCommandLine);
							args_.push_back(now);
						}
						clearS(now);

						pStart = p;// = skipWS(p);
						continue;
					}
					else
					{
						// not in DQ, not WS (=normal char)
						if (isDQ(*p))
						{
							// not in DQ, DQ starts
							inDQ = true;
							continue;
						}
						else
						{
							// not in DQ, not WS, not DQ
							now += (*p);
							continue;
						}
					}
				}
				else
				{
					// inDQ
					if (isDQ(*p))
					{
						if (isDQ(*(p + 1)))
						{
							// inDQ, continuous DQ
							now += (*p);
							p++;
							continue;
						}
						else
						{
							// inDQ, not CDQ, DQ ends
							if (!now.empty())
							{
								offsets_.push_back(pStart - pCommandLine);
								args_.push_back(now);
							}
							clearS(now);
							inDQ = false;
							continue;
						}
					}
					else
					{
						// inDQ, not DQ
						now += *p;
						continue;
					}
				}
				// all condition must end with continue.
				assert(false);
			}
			if (!now.empty())
			{
				offsets_.push_back(pStart - pCommandLine);
				args_.push_back(now);
			}
			//assert(p_ == pOOO);
			//free(p_);
		}
		

	public:
		CCommandLineStringBase()
		{
			init(GetCommandLine());
		}
		CCommandLineStringBase(const E* pCommandLine)
		{
			if (!*pCommandLine)
			{
				E e[MAX_PATH];
				GetModuleFileNameA(NULL, e, _countof(e));
				init(e);
			}
			else
			{
				init(pCommandLine);
			}
		}
		~CCommandLineStringBase()
		{
			delete[] (E*)p_;
		}
		int getIndex(const E* p) const
		{
			for (unsigned int i = 0; i < args_.size(); ++i)
			{
				if (args_[i] == p)
					return i;
			}
			return -1;
		}

		std::basic_string<E> subString(int index) const
		{
			if (offsets_.size() == index)
				return L"";

			int ofs = offsets_[index];
			std::basic_string<E> ret(&p_[ofs]);
			const E* pTrimming = ret.c_str();
			ret = skipWS(pTrimming);
			return ret;
		}

		static E** getCommandLine(const E* pCL, int* argc)
		{
			CCommandLineStringBase<E> me(pCL);

			*argc = 0;
			E** ppRetReturn = (E**)LocalAlloc(0, (me.args_.size() + 1) * sizeof(E*));
			E** ppRet = ppRetReturn;
			int i = 0;
			for (myVS::iterator it = me.args_.begin();
				it != me.args_.end(); ++it)
			{
				if (it->empty())
					continue;

				*ppRet = (E*)LocalAlloc(0, (it->size() + 1) * sizeof(E));
				// wcscpy_s(*ppRet, LocalSize(*ppRet)/sizeof(E), it->c_str());
				// _tcscpy(*ppRet, it->c_str());
				memcpy(*ppRet, it->c_str(), (it->size()+1)*sizeof(E));
				++ppRet;
				++i;
			}
			*argc = i;
			*ppRet = NULL;
			return ppRetReturn;
		}
		static void freeCommandLine(E** argv)
		{
			for (E** pp = argv; *pp; ++pp)
			{
				LocalFree(*pp);
			}
			LocalFree(argv);
		}
	};

#ifdef UNICODE
	typedef CCommandLineStringBase<wchar_t> CCommandLineString;
#endif
}