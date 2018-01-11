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

#include <Windows.h>
#include <string>
#include <vector>



namespace Ambiesoft {
	class CSingleAppMutex
	{
		HANDLE h_;
		std::wstring name_;
		DWORD dwLE_;
	public:
		CSingleAppMutex(const wchar_t* pName)
		{
			name_ = pName;
			h_ = CreateMutex(NULL, TRUE, pName);
			dwLE_ = GetLastError();
		}
		~CSingleAppMutex()
		{
			CloseHandle(h_);
		}
		bool IsFirst() const
		{
			return h_ != nullptr && dwLE_ == ERROR_SUCCESS;
		}
		bool IsSecond() const
		{
			return h_ != nullptr && dwLE_ == ERROR_ALREADY_EXISTS;
		}
		bool IsOK() const
		{
			return h_ != nullptr;
		}
		static std::vector<HWND> GetExistingWindow(const wchar_t* pTitle, const wchar_t* pClassName);
		static bool SendInfo(HWND hwnd, HWND hSender, ULONG_PTR category, const unsigned char* data, unsigned int len);
		static void ReceiveInfo(WPARAM wParam, LPARAM lParam, HWND* phSender, ULONG_PTR* pCategory, unsigned int* pLen, unsigned char** pData);
	};
}