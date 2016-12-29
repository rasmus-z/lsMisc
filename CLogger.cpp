#include "stdafx.h"
#include "stdwin32/stdwin32.h"
#include "CLogger.h"

namespace Ambiesoft {

using namespace stdwin32;


HANDLE CLogger::m_hLog;
CLogger::CLogger()
{

}

CLogger::~CLogger()
{
	if(m_hLog)
	{
		CloseHandle(m_hLog);
		m_hLog=NULL;
	}
}

bool CLogger::Prepare()
{
	tstring file = stdGetFileName(stdGetModuleFileName()) + _T(".log");

	m_hLog = CreateFile(file.c_str(),
		FILE_APPEND_DATA,
		FILE_SHARE_READ, // share
		NULL, // sec
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (m_hLog == INVALID_HANDLE_VALUE)
	{
		m_hLog = NULL;
		return false;
	}

	return true;
}

bool CLogger::Out(LPCSTR p1, DWORD d1)
{
	char szT[32];
	wsprintfA(szT, "%d", d1);

	string s(p1);
	s += szT;

	return Out(s.c_str());
}

bool CLogger::Out(LPCSTR pSTR)
{
	if (!m_hLog)
		return false;

	CHAR buff[256]; buff[0]=0;
	time_t now = time(NULL);
	struct tm *pnow = localtime(&now);
	strftime(buff, countof(buff), "%X\t", pnow);

	DWORD dwWritten = 0;
	BOOL bFailed = false;

		
	dwWritten = 0;
	bFailed = !WriteFile(m_hLog,
		buff,
		lstrlenA(buff)*sizeof(CHAR),
		&dwWritten,
		NULL);

	dwWritten = 0;
	bFailed = !WriteFile(m_hLog,
		pSTR,
		lstrlenA(pSTR)*sizeof(CHAR),
		&dwWritten,
		NULL);

	if (true)
	{
		bFailed |= !WriteFile(m_hLog,
			"\r\n",
			2,
			&dwWritten,
			NULL);
	}

	return (!bFailed);
}



}
