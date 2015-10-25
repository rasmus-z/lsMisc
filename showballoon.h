BOOL showballoon(
				 HWND hWnd, 
				 const std::wstring& title, 
				 const std::wstring& text, 
				 HICON hIcon, 
				 int duration,
				 UINT uTrayID,
				 BOOL bOnlyModify=FALSE,
				 DWORD dwBalloonIcon=0);