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


#include <map>

template<typename T> class CTlsMap {	
private:
	std::map<DWORD, T*> m_map;
	CRITICAL_SECTION m_cs;

public:
	CTlsMap() {
		InitializeCriticalSection(&m_cs);
	}
	~CTlsMap() {
		DeleteCriticalSection(&m_cs);
	}


	struct CSFreer_t {
		CRITICAL_SECTION* m_pCS;
		CSFreer_t(CRITICAL_SECTION& cs) {
			m_pCS = &cs;
			EnterCriticalSection(&cs);
		}
		~CSFreer_t() {
			LeaveCriticalSection(m_pCS);
		}
	};

	T* getInstance() {
		CSFreer_t freer(m_cs);
		std::map<DWORD, T*>::iterator it = m_map.find(GetCurrentThreadId());
		ASSERT(it != m_map.end());
		return it->second;
	}

	void setInstance(T* pDnab) {
		CSFreer_t freer(m_cs);
		m_map.insert(std::pair<DWORD, T*>(GetCurrentThreadId(), pDnab));
	}

	size_t GetCount() {
		CSFreer_t freer(m_cs);
		return m_map.size();
	}
} ;
