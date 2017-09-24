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
#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#endif

#include <string>

namespace Ambiesoft {


	struct os_traits_windows_ansi
	{};
	struct os_traits_windows_unicode
	{};
	struct os_traits_gcc
	{};
#ifdef _WIN32
#ifdef UNICODE
	#pragma message("os trait is Windows unicode")
	typedef os_traits_windows_unicode current_traits;
	typedef std::wstring myS_;
#else
	#pragma message("os trait is Windows ansii")
	typedef os_traits_windows_ansi current_traits;
	typedef std::string myString;
#endif

#elif __linux__
	#pragma message("__linux__")
#elif __GNUC__
	#pragma message("GNUC")
	typedef os_traits_gcc current_traits;
	typedef std::string myS_;
#else
	#pragma message("NAZO")
#endif

	template<class E, class S>
	struct os_trais
	{
		typedef typename S::traits_type::char_type myE_;
		static S osdGetEmptyString()
		{
			return S();
		}
		static S osdGetModuleFileName()
		{
			return "";
		}
		static bool osdIsFullPath(const S& s)
		{
			return false;
		}
		static bool osdIsFullPath(const myE_* p)
		{
			return false;
		}
		static myE_ osdGetPathSeparator()
		{
			return '/';
		}
		static S osdCombinePath(const S& left, const S& right)
		{
			if (osdIsFullPath(right))
				return right;

			S ret(left);
			typename S::iterator it = ret.end();
			--it;
			if (*it != osdGetPathSeparator())
				ret += osdGetPathSeparator();

			return ret + right;
		}
	};

	template<>
	struct os_trais<os_traits_windows_ansi, std::string>
	{
		typedef std::string S;
		typedef std::string::traits_type::char_type myE_;
		static S osdGetEmptyString()
		{
			return S();
		}
		static std::string osdGetModuleFileName()
		{
			char t[MAX_PATH];
			GetModuleFileNameA(NULL, t, MAX_PATH);
			return t;
		}
		static bool osdIsFullPath(const S& s)
		{
			osdIsFullPath(s.c_str());
		}
		static bool osdIsFullPath(const myE_* p)
		{
			return !PathIsRelativeA(p);
		}
		static myE_ osdGetPathSeparator()
		{
			return '\\';
		}
		static S osdCombinePath(const S& left, const S& right)
		{
			if (osdIsFullPath(right))
				return right;

			S ret(left);
			S::iterator it = ret.end();
			--it;
			if (*it != osdGetPathSeparator())
				ret += osdGetPathSeparator();

			return ret + right;
		}
	};

	template<>
	struct os_trais<os_traits_windows_unicode, std::wstring>
	{
		typedef std::wstring S;
		typedef S::traits_type::char_type myE_;
		static S osdGetModuleFileName()
		{
			wchar_t t[MAX_PATH];
			GetModuleFileNameW(NULL,t,MAX_PATH);
			return t;
		}
		static bool osdIsFullPath(const S& s)
		{
			return osdIsFullPath(s.c_str());
		}
		static bool osdIsFullPath(const myE_* p)
		{
			return !PathIsRelative(p);
		}
		static myE_ osdGetPathSeparator()
		{
			return L'\\';
		}
		static S osdCombinePath(const S& left, const S& right)
		{
			if (osdIsFullPath(right))
				return right;

			S ret(left);
			S::iterator it = ret.end();
			--it;
			if (*it != osdGetPathSeparator())
				ret += osdGetPathSeparator();

			return ret + right;
		}
	};

	template<>
	struct os_trais<os_traits_gcc, std::string>
	{
		typedef std::wstring S;
		typedef S::traits_type::char_type myE_;
		static S osdGetModuleFileName()
		{
			return S();
		}
		static bool osdIsFullPath(const S& s)
		{
			return osdIsFullPath(s.c_str());
		}
		static bool osdIsFullPath(const myE_* p)
		{
			return !PathIsRelative(p);
		}
		static myE_ osdGetPathSeparator()
		{
			return L'\\';
		}
		static S osdCombinePath(const S& left, const S& right)
		{
			if (osdIsFullPath(right))
				return right;

			S ret(left);
			S::iterator it = ret.end();
			--it;
			if (*it != osdGetPathSeparator())
				ret += osdGetPathSeparator();

			return ret + right;
		}
	};

	template<class OsT_ = current_traits>
	myS_ mGetModuleFileName()
	{
		return os_trais<OsT_, myS_>::osdGetModuleFileName();
	}

	template<class OsT_ = current_traits>
	bool mIsFullPath(const myS_& s)
	{
		return os_trais<OsT_, myS_>::osdIsFullPath(s);
	}

	template<class OsT_ = current_traits>
	myS_ mCombinePath(const myS_& left, const myS_& right)
	{
		return os_trais<OsT_, myS_>::osdCombinePath(left, right);
	}

} // namespace
