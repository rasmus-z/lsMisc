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
namespace Ambiesoft {


	struct os_traits_windows_ansi
	{};
	struct os_traits_windows_unicode
	{};

#ifdef _WIN32
#ifdef UNICODE
	#pragma message("os trait is Windows unicode")
	typedef os_traits_windows_unicode current_traits;
	typedef std::wstring myString;
#else
	#pragma message("os trait is Windows ansii")
	typedef os_traits_windows_ansi current_traits;
	typedef std::string myString;
#endif
#endif

	template<class E, class S>
	struct os_trais
	{
		static S osdGetModuleFileName()
		{
			return "";
		}
	};

	template<>
	struct os_trais<os_traits_windows_ansi, std::string>
	{
		static std::string osdGetModuleFileName()
		{
			return "";
		}
	};
	template<>
	struct os_trais<os_traits_windows_unicode, std::wstring>
	{
		static std::wstring osdGetModuleFileName()
		{
			wchar_t t[MAX_PATH];
			GetModuleFileNameW(NULL,t,MAX_PATH);
			return t;
		}
	};

	myString mGetModuleFileName()
	{
		myString s = os_trais<current_traits, myString>::osdGetModuleFileName();
		return s;
	}

} // namespace