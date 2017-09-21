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

#include "StdAfx.h"
#include <Windows.h>


#include "Is64.h"

namespace Ambiesoft {

	typedef void(WINAPI *LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO);
	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

	bool Is64BitWindows()
	{
#if defined(_WIN64)
		return true;  // 64-bit programs run only on Win64
#elif defined(_WIN32)

		// 32-bit programs run on both 32-bit and 64-bit Windows
		// so must sniff
		BOOL f64 = FALSE;
		LPFN_ISWOW64PROCESS fnIsWow64Process;

		fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
		if (NULL != fnIsWow64Process)
		{
			return !!(fnIsWow64Process(GetCurrentProcess(), &f64) && f64);
		}

		return false;
#else
		return false; // Win64 does not support Win16
#endif
	}

#ifndef PROCESSOR_ARCHITECTURE_AMD64
#define PROCESSOR_ARCHITECTURE_AMD64 9
#endif

	typedef void(WINAPI *LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO);
	LPCWSTR GetPlatformW()
	{
		static WCHAR sT[64];
		static bool done = false;
		if (done)
			return sT;

		LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)GetProcAddress(
			GetModuleHandle(TEXT("kernel32")), "GetNativeSystemInfo");

		if (NULL == fnGetNativeSystemInfo)
		{
			lstrcpy(sT, L"Unknown");
		}
		else
		{
			SYSTEM_INFO si = { 0 };
			fnGetNativeSystemInfo(&si);
			switch (si.wProcessorArchitecture)
			{
			case PROCESSOR_ARCHITECTURE_AMD64:	lstrcpy(sT, L"AMD64");	break;
			case PROCESSOR_ARCHITECTURE_ARM:	lstrcpy(sT, L"ARM");	break;
			case PROCESSOR_ARCHITECTURE_IA64:	lstrcpy(sT, L"IA64");	break;
			case PROCESSOR_ARCHITECTURE_INTEL:	lstrcpy(sT, L"INTEL");	break;
			default:lstrcpy(sT, L"Unknown"); break;
			}
		}
		return sT;
	}

	bool Is64BitProcess()
	{
#if defined(_WIN64)
		return true;
#else
		return false;
#endif
	}
}
