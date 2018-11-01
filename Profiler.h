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
#ifdef AMBIESOFT_PROFILER_ENABLED

#include <Windows.h>
#include <stdlib.h>
#include <string>

namespace Ambiesoft
{
	class CProfiler
	{
		DWORD tick_ = 0;
		FILE* file_ = NULL;
	private:
		CProfiler();
		~CProfiler();


	public:
		static CProfiler& GetInstance() {
			static CProfiler theInstance;
			return theInstance;
		}

		void Stamp(const std::string& message, const char* filename, int line);
		void Close(const std::string& message, const char* filename, int line);
	};
}

#define AMBIESOFT_PROFILER(message) do {CProfiler::GetInstance().Stamp(message, __FILE__,__LINE__);} \
__pragma(warning(push)) \
__pragma(warning(disable:4127)) \
					while (false) \
__pragma(warning(pop))

#define AMBIESOFT_PROFILER_CLOSE(message) do {CProfiler::GetInstance().Close(message, __FILE__,__LINE__);} \
__pragma(warning(push)) \
__pragma(warning(disable:4127)) \
					while (false) \
__pragma(warning(pop))

#else // AMBIESOFT_PROFILER_ENABLED
#define AMBIESOFT_PROFILER(message) (void)0
#define AMBIESOFT_PROFILER_CLOSE(message) (void)0
#endif // AMBIESOFT_PROFILER_ENABLED