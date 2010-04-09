#include <comdef.h>
#include "AnyCloser.h"

bool addBaseElement(_bstr_t& bstrHtml, LPCWSTR pSourceURL)
{
	if ( pSourceURL==NULL || pSourceURL[0] == 0 )
		return false;

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

		ret += L"<base href=\"";
		ret += pSourceURL;
		ret += L"\" />";

		*p = c;
		ret += p;
		bstrHtml = ret;
		return true;
	}
	else
	{
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

			ret += L"<head><base href=\"";
			ret += pSourceURL;
			ret += L"\" /></head>";

			*p = c;
			ret += p;
			bstrHtml = ret;
			return true;
		}
	}
	return false;
}





















//bool addBaseElement(_bstr_t& bstrHtml, LPCWSTR pSourceURL)
//{
//	if ( !pSourceURL )
//		return false;
//
//	WCHAR* pW = wcsdup(bstrHtml);
//	if(!pW)
//		return false;
//
//	struct strfreer_t {
//		WCHAR* p;
//		strfreer_t(WCHAR* q) : p(q) {}
//		~strfreer_t() {
//			free((void*)p);
//		}
//	} strfreer(pW);
//
//	WCHAR* p = wcsstr(pW, L"</head");
//	if ( p==NULL )
//		p = wcsstr(pW, L"</HEAD");
//	if ( p==NULL )
//		p = wcsstr(pW, L"</Head");
//	if ( p==NULL )
//	{ // could not find head tag, try to find body tag
//		p = wcsstr(pW, L"<body");
//		if ( p==NULL )
//			p = wcsstr(pW, L"<BODY");
//		if ( p==NULL )
//			p = wcsstr(pW, L"<Body");
//		if ( p==NULL )
//		{
//			ASSERT(false);
//			return false;
//		}
//
//		// body tag found.
//		_bstr_t ret;
//		*p = 0;
//		ret = pW;
//		*p = L'<';
//
//		ret += L"<head><base href=\"";
//		ret += pSourceURL;
//		ret += L"\" /></head>";
//
//		ret += p;
//		bstrHtml = ret;
//		return true;
//	}
//	else
//	{ // head tag found.
//		_bstr_t ret;
//		*p = 0;
//		ret = pW;
//		*p = L'<';
//
//		ret += L"<base href=\"";
//		ret += pSourceURL;
//		ret += L"\" />";
//
//		ret += p;
//
//		bstrHtml = ret;
//		return true;
//	}
//	ASSERT(false);
//	return false;
//}