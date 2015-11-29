// WaitCursor.cpp: CWaitCursor クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaitCursor.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
int CWaitCursor::m_nCount = 0;
LPCRITICAL_SECTION CWaitCursor::ms_pcsCursor = NULL;

void CWaitCursor::DoWait(BOOL bWait)
{
	if (NULL==ms_pcsCursor)
	{
		ms_pcsCursor = new CRITICAL_SECTION;
		InitializeCriticalSection(ms_pcsCursor);

		atexit(DeleteCursorCS);
	}
	
	
	EnterCriticalSection(ms_pcsCursor);
	if(bWait)
	{
		if(m_nCount > 0)
		{
			m_nCount++;
			LeaveCriticalSection(ms_pcsCursor);
			return;
		}
		m_nCount = 1;

		//砂時計カーソルにする
		m_hCurOld = SetCursor(LoadCursor(NULL, IDC_WAIT));
	}
	else
	{
		if(m_nCount > 1){
			m_nCount--;
			LeaveCriticalSection(ms_pcsCursor);
			return;
		}
		m_nCount = 0;

		if( m_hCurOld == NULL){
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}
		//元のカーソルに戻す
		SetCursor(m_hCurOld);
		m_hCurOld = NULL;
	}
	LeaveCriticalSection(ms_pcsCursor);
}

void CWaitCursor::DeleteCursorCS(void) 
{
	assert(ms_pcsCursor);
	DeleteCriticalSection(ms_pcsCursor);
	delete ms_pcsCursor;
	ms_pcsCursor = NULL;
}