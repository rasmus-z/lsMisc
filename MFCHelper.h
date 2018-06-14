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

#pragma once
#include <afxwin.h>

CString GetFavoritesFolder();
//BOOL CreateInterShortcutHelper (LPCTSTR pURL, 
//								LPCTSTR pDir, 
//								LPCTSTR pTitle);
//
//HRESULT CreateInterShortcut (LPCTSTR pszURL, 
//							 LPCTSTR pszURLfilename,
//							 LPCTSTR szDescription,
//							 LPCTSTR szIconFile = NULL,
//							 int nIndex = -1);

struct IHTMLDocument2;
typedef BOOL (*WALKIEDOCFUNC)(IHTMLDocument2* pDoc, void* pVoid);
int WalkIEDocument(WALKIEDOCFUNC f, void* pVoid);
// BOOL CreateCompleteDir(LPCTSTR lpszDir, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);

struct showerror_t {
private:
	CString m_strMessage;
public:
	showerror_t(LPCTSTR s) {
		m_strMessage = s;
	}
	~showerror_t() {
		if(!m_strMessage.IsEmpty())
		{
			AfxMessageBox(m_strMessage);
		}
	}
	void cancel() {
		m_strMessage = _T("");
	}
	
	operator bool() const {
		return m_strMessage.IsEmpty() == FALSE;
	}
};


CString dqIfSpace(const CString& s);


//bool HasDupPaths(LPCTSTR pLeft, const CStringArray& saPaths, int startindex, int& hitindex, CString& common);
//bool checkDupPaths(const CStringArray& saPaths, CString&left, CString& right, CString&common);

void getLinesFromEdit(CEdit& edit, CStringArray& ar, bool bRaw);