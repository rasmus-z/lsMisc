#pragma once

namespace Ambiesoft {


class CLogger
{
	CLogger();
	~CLogger();

	static HANDLE m_hLog;
public:
	static void Prepare();
	static bool Out(LPCSTR pSTR, BOOL bAppendRet = FALSE);
};










}