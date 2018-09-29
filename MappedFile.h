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
	class CMappedFile
	{
		HANDLE f_;
		HANDLE m_;
		char* p_;
		DWORD dwLE;

	public:
		CMappedFile() : f_(INVALID_HANDLE_VALUE), m_(NULL), p_(NULL), dwLE(0) {}
		~CMappedFile() {
			Close();
		}
		void Close()
		{
			if (p_)
			{
				UnmapViewOfFile(p_);
				p_ = NULL;
			}
			if (m_)
			{
				CloseHandle(m_);
				m_ = NULL;
			}
			if (f_ != INVALID_HANDLE_VALUE)
			{
				CloseHandle(f_);
				f_ = INVALID_HANDLE_VALUE;
			}
			dwLE = 0;
		}
		LONGLONG size() {
			if (f_ == INVALID_HANDLE_VALUE)
				return 0;

			LARGE_INTEGER li;
			if (!GetFileSizeEx(f_, &li))
				return 0;

			return li.QuadPart;
		}
		bool Open(LPCWSTR pFilename)
		{
			Close();
			f_ = CreateFile(
				pFilename,
				GENERIC_READ,// | GENERIC_WRITE, 
				0,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_RANDOM_ACCESS,
				NULL);
			if (f_ == INVALID_HANDLE_VALUE)
			{
				dwLE = GetLastError();
				return false;
			}

			m_ = CreateFileMapping(
				f_,
				NULL,
				PAGE_READONLY, //WRITE | SEC_RESERVE,
				0,
				0,
				0);
			if (m_ == NULL)
			{
				dwLE = GetLastError();
				return false;
			}

			p_ = (char*)MapViewOfFile(
				m_,
				FILE_MAP_READ,
				0,
				0,
				0);
			if (p_ == NULL)
			{
				dwLE = GetLastError();
				return false;
			}

			return true;
		}

		char* ptr() {
			return p_;
		}

		DWORD getError() const {
			return dwLE;
		}
	};

}