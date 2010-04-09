#pragma once

class CHtmlClipboard
{
private:
	CHtmlClipboard(void){}
	~CHtmlClipboard(void){}

public:
	static bool getHtmlFromClipboard(HWND hWnd, _bstr_t* pHtml, _bstr_t* Title = NULL, _bstr_t* URL = NULL);
	static bool CHtmlClipboard::getHtml(LPSTR p, _bstr_t* pHtml, _bstr_t* pTitle, _bstr_t* pURL);

	static UINT getClipboardFormat(void);
};
