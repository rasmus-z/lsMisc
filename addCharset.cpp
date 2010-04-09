#include <comdef.h>
#include "AnyCloser.h"

bool addCharset(_bstr_t& bstrHtml)
{
	WCHAR* pw = wcsdup(bstrHtml);
	if(!pw)
		return false;

//	struct strfreer_t {
//		WCHAR* p;
//		strfreer_t(WCHAR* q) : p(q) {}
//		~strfreer_t() {
//			free((void*)p);
//		}
//	} strfreer(pw);
	CRTFreer freer(pw);

	LPWSTR p = (LPWSTR)wcsstr(pw, L"<head");
	if ( p==NULL )
		p = wcsstr(pw, L"<HEAD");
	if ( p==NULL )
		p = wcsstr(pw, L"<Head");
	if ( p )
	{
		while(*p && *p != L'>')
			p++;
		p++;

		_bstr_t ret;

		WCHAR c = *p;
		*p = 0;
		ret += pw;
		ret += "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">";
		*p = c;
		ret += p;
		bstrHtml = ret;
		return true;
	}
	else
	{  // head tag notfound
		LPWSTR p = (LPWSTR)wcsstr(pw, L"<html");
		if ( p==NULL )
			p = wcsstr(pw, L"<HTML");
		if ( p==NULL )
			p = wcsstr(pw, L"<Html");

		if ( p )
		{
			while(*p && *p != L'>')
				p++;
			p++;

			_bstr_t ret;

			WCHAR c = *p;
			*p = 0;
			ret += pw;
			ret += "<head><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\"></head>";
			*p = c;
			ret += p;
			bstrHtml = ret;
			return true;
		}
	}
			
	return false;
}
