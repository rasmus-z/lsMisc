#pragma once

namespace Ambiesoft {


class CLogger
{
	CLogger();
	~CLogger();

	static HANDLE m_hLog;
public:
	static bool Prepare();
	static bool IsReady() {
		return m_hLog != NULL;
	}
	static bool Out(LPCSTR p1);
	static bool Out(LPCSTR p1, DWORD d1);
};










}