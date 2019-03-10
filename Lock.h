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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace Ambiesoft {
	class CLock
	{
	public:
		CLock(LPCRITICAL_SECTION pSection) {
			assert(pSection);
			m_pSection = pSection;
			EnterCriticalSection(pSection);
		}

		~CLock() {
			LeaveCriticalSection(m_pSection);
		}
	protected:
		LPCRITICAL_SECTION m_pSection;
	};

	class CReadWriteLock
	{
		SRWLOCK lock_;
	public:
		CReadWriteLock();
		void acRead();
		void acReadWrite();
		void rlRead();
		void rlReadWrite();
	};

	class CReadWriteLocker
	{
		CReadWriteLock& lock_;
	public:
		enum class LockType {
			READ_LOCK,
			READWRITE_LOCK,
		} lockType_;
		CReadWriteLocker(CReadWriteLock& lock, LockType lt) :
			lock_(lock), lockType_(lt){
			if (lt == LockType::READ_LOCK)
				lock_.acRead();
			else if (lt == LockType::READWRITE_LOCK)
				lock_.acReadWrite();
			else
				assert(false);
		}
		~CReadWriteLocker()
		{
			if (lockType_ == LockType::READ_LOCK)
				lock_.rlRead();
			else if (lockType_ == LockType::READWRITE_LOCK)
				lock_.rlReadWrite();
			else
				assert(false);
		}
	};

} // namespace Ambiesoft