CString GetFavoritesFolder();
BOOL CreateInterShortcutHelper (LPCTSTR pURL, 
								LPCTSTR pDir, 
								LPCTSTR pTitle);

HRESULT CreateInterShortcut (LPCTSTR pszURL, 
							 LPCTSTR pszURLfilename,
							 LPCTSTR szDescription,
							 LPCTSTR szIconFile = NULL,
							 int nIndex = -1);

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
