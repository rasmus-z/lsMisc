#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

