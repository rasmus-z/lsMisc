#pragma once

namespace Ambiesoft {

template<class T>
class CSessionGlobalMemory
{
public:
	explicit CSessionGlobalMemory(LPCSTR pName) {
		init(pName);
	}
	CSessionGlobalMemory(const CSessionGlobalMemory& rhv)
	{
		init(rhv.m_pName);
	}
	CSessionGlobalMemory(CSessionGlobalMemory&& rhv)
	{
		move(rhv);
	}

	void move(CSessionGlobalMemory&& rhv) {
		m_pName = rhv.m_pName;
		rhv.m_pName = NULL;

		m_pMutexName = rhv.m_pMutexName;
		rhv.m_pMutexName = NULL;

		m_ = rhv.m_;
		rhv.m_ = NULL;

		h_ = rhv.h_;
		rhv.h_ = NULL;

		p_ = rhv.p_;
		rhv.p_ = NULL;
	}
	void operator=(const CSessionGlobalMemory& rhv) {
		if(this==&rhv)
			return;

		release();
		init(rhv.m_pName);
	}
	CSessionGlobalMemory& operator=(CSessionGlobalMemory&& rhv) {
		if(this==&rhv)
			return *this;

		move(rhv);
		return *this;
	}

	~CSessionGlobalMemory() {
		release();
	}

	operator T() {
		ensure();
		Locker l(m_);
		//T t;
		//memcpy(&t, p_, sizeof(t));

		return *((T*)p_);
	}

	void get(T* pt) {
		ensure();
		Locker l(m_);
		memcpy(pt, p_, sizeof(t));
	}

	CSessionGlobalMemory& operator =(const T& t) {
		set(t);
		return *this;
	}

protected:
	void* p_;
	CSessionGlobalMemory() {}
	void init(LPCSTR pName) {
		h_ = NULL;
		p_ = NULL;
		m_ = NULL;

		size_t len = lstrlenA(pName);
		m_pName = (LPSTR)LocalAlloc(LMEM_FIXED, len + sizeof(char));
		lstrcpyA(m_pName, pName);

		m_pMutexName = (LPSTR)LocalAlloc(LMEM_FIXED, len + sizeof("_Mutex") - 1 + sizeof(char));
		lstrcpyA(m_pMutexName, pName);
		lstrcatA(m_pMutexName, "_Mutex");
	}
	void release() {
		if (p_)
		{
			UnmapViewOfFile(p_);
			p_ = NULL;
		}

		if (h_)
		{
			CloseHandle(h_);
			h_ = NULL;
		}

		if (m_)
		{
			CloseHandle(m_);
			m_ = NULL;
		}
		LocalFree(m_pName);
		LocalFree(m_pMutexName);
	}
	void ensure() {
		if (!m_)
		{
			m_ = CreateMutexA(NULL, FALSE, m_pMutexName);
		}

		bool first = false;
		if (!h_)
		{
			h_ = CreateFileMappingA(
				INVALID_HANDLE_VALUE,
				NULL,
				PAGE_READWRITE,
				0, sizeof(T),
				m_pName);
			if (h_ && GetLastError() != ERROR_ALREADY_EXISTS)
			{
				first = true;
			}
		}
		if (!p_)
		{
			p_ = MapViewOfFile(
				h_,
				FILE_MAP_READ | FILE_MAP_WRITE,
				0, 0,
				sizeof(T));
		}

		if (first)
		{
			Locker l(m_);
			if (first)
			{
				memset(p_, 0, sizeof(T));
				first = false;
			}
		}
	}

	//T get() {
	//}


	void set(const T& t) {
		ensure();
		Locker l(m_);
		memcpy(p_, &t, sizeof(t));
	}

private:
	class Locker {
	public:
		HANDLE m_;
		Locker(HANDLE m) {
			m_ = m;
			WaitForSingleObject(m, INFINITE);
		}
		~Locker() {
			ReleaseMutex(m_);
		}
	};


	bool first_;
	HANDLE h_;
	HANDLE m_;
	LPSTR m_pName;
	LPSTR m_pMutexName;

	// prohibitted, use NTS
	T* operator &() {}
};

// Non Thread Safe
template<class T>
class CSessionGlobalMemoryNTS : CSessionGlobalMemory<T>
{
public:
	explicit CSessionGlobalMemoryNTS(LPCSTR pName) {
		init(pName);
	}
	T* operator &() {
		ensure();
		return (T*)p_;
	}
};



}  // namespace
