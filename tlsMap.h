
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
