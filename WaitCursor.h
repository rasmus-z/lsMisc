// WaitCursor.h: CWaitCursor クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAITCURSOR_H__A8305580_93C4_11D4_8DFC_C43BFFBA435F__INCLUDED_)
#define AFX_WAITCURSOR_H__A8305580_93C4_11D4_8DFC_C43BFFBA435F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWaitCursor  
{
public:
	CWaitCursor()
	{
		DoWait(TRUE);
	}

	~CWaitCursor()
	{
		DoWait(FALSE);
	}

private:
	static LPCRITICAL_SECTION ms_pcsCursor;
	static void DeleteCursorCS(void);
	void DoWait(BOOL bWait);
	static int m_nCount;
	HCURSOR m_hCurOld;
};

#endif // !defined(AFX_WAITCURSOR_H__A8305580_93C4_11D4_8DFC_C43BFFBA435F__INCLUDED_)
