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

#include "stdafx.h"

#include <cassert>
#include <stlsoft/smartptr/scoped_handle.hpp>

#include "stlScopedClear.h"
#include "RunCommandGetResult.h"
#include "DebugNew.h"

namespace Ambiesoft {
	
	static BOOL CreateHandles(HANDLE& h1, HANDLE& h2, BOOL i1, BOOL i2, DWORD* pdwLastError)
	{
		SECURITY_ATTRIBUTES sa;

		ZeroMemory(&sa, sizeof(sa));
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;

		if (!CreatePipe(&h1, &h2, &sa, 0))
		{
			if (pdwLastError)
				*pdwLastError = GetLastError();
			return FALSE;
		}

		if (!i1)
		{
			if (!SetHandleInformation(h1, HANDLE_FLAG_INHERIT, 0))
			{
				if (pdwLastError)
					*pdwLastError = GetLastError();
				return FALSE;
			}
		}
		if (!i2)
		{
			if (!SetHandleInformation(h2, HANDLE_FLAG_INHERIT, 0))
			{
				if (pdwLastError)
					*pdwLastError = GetLastError();
				return FALSE;
			}
		}

#ifdef _DEBUG
		{
			// check inheritance
			DWORD dwFlags = 0;
			if (!GetHandleInformation(h1, &dwFlags))
				assert(false);
			assert((i1 && (dwFlags & HANDLE_FLAG_INHERIT)) || (!i1 && (dwFlags & HANDLE_FLAG_INHERIT) == 0));

			if (!GetHandleInformation(h2, &dwFlags))
				assert(false);
			assert((i2 && (dwFlags & HANDLE_FLAG_INHERIT)) || (!i2 && (dwFlags & HANDLE_FLAG_INHERIT) == 0));
		}
#endif
		return TRUE;
	}
	BOOL RunCommandGetResult(LPCWSTR pExe, LPCWSTR pArg, int* pIRetCommand, std::string* pStrRetCommand, DWORD* pdwLastError)
	{
		HANDLE hPipeStdInRead = NULL;
		HANDLE hPipeStdInWrite = NULL;
		if (!CreateHandles(hPipeStdInRead, hPipeStdInWrite, TRUE, FALSE, pdwLastError))
			return FALSE;
		STLSOFT_SCOPEDFREE_HANDLE(hPipeStdInRead);
		STLSOFT_SCOPEDFREE_HANDLE(hPipeStdInWrite);

		HANDLE hPipeStdOutRead = nullptr;
		HANDLE hPipeStdOutWrite = nullptr;
		if (!CreateHandles(hPipeStdOutRead, hPipeStdOutWrite, FALSE, TRUE, pdwLastError))
			return FALSE;
		STLSOFT_SCOPEDFREE_HANDLE(hPipeStdOutRead);
		STLSOFT_SCOPEDFREE_HANDLE(hPipeStdOutWrite);

		HANDLE hPipeStdErrRead = nullptr;
		HANDLE hPipeStdErrWrite = nullptr;
		if (!CreateHandles(hPipeStdErrRead, hPipeStdErrWrite, FALSE, TRUE, pdwLastError))
			return FALSE;
		STLSOFT_SCOPEDFREE_HANDLE(hPipeStdErrRead);
		STLSOFT_SCOPEDFREE_HANDLE(hPipeStdErrWrite);

		// createprocess
		LPWSTR pArgCP = pArg ? _wcsdup(pArg) : NULL;
		STLSOFT_SCODEDFREE_CRT(pArgCP);

		STARTUPINFO siStartInfo = { 0 };
		siStartInfo.cb = sizeof(STARTUPINFO);
		siStartInfo.hStdInput = hPipeStdInRead;
		siStartInfo.hStdOutput = hPipeStdOutWrite;
		siStartInfo.hStdError = hPipeStdErrWrite;
		siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

		PROCESS_INFORMATION pi = { 0 };
		if(!CreateProcess(
			pExe,
			pArgCP,
			nullptr, // sec
			nullptr, // sec thread
			TRUE, // inherit
			0, // flags
			nullptr, // env
			nullptr, // dir
			&siStartInfo,
			&pi))
		{
			if (pdwLastError)
				*pdwLastError = GetLastError();
			return FALSE;
		}

		CreateThread();
		return TRUE;
	}
}