BOOL vbregMatch(LPCTSTR subject, LPCTSTR regex);
LPCTSTR vbregReplace(LPCTSTR source,
					 LPCTSTR replace, 
					 LPCTSTR regex,
					 BOOL bGlobal,
					 BOOL bCaseInsensitive);