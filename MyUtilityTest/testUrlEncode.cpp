#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cassert>
#include <stlsoft/smartptr/scoped_handle.hpp>

#include "../UrlEncode.h"
#include "../UTF16toUTF8.h"

using namespace std;

void testUrlEncode()
{
	// ignore leak

	//assert(0 == strcmp(UrlEncode((LPBYTE)"aaa"), "aaa"));
	//assert(0 == strcmp(UrlEncode((LPBYTE)"aaa a"), "aaa+a"));

	//assert(0 == wcscmp( UrlEncode(L"aaa"), L"aaa"));
	//assert(0 == wcscmp( UrlEncode(L"aaa a"), L"aaa+a"));
	
	{
		LPCSTR p = "https://social.msdn.microsoft.com/Search/en-US?query=isalnum&beta=0&ac=8";
		LPSTR penc = UrlEncode((LPBYTE)p);
		stlsoft::scoped_handle<void*> mapenc(penc, free);

		LPBYTE pdec = UrlDecode(penc);
		stlsoft::scoped_handle<void*> mapdec(pdec, free);
		assert(strcmp(p, (LPCSTR)pdec) == 0);
	}

	{
		LPCSTR p = "https://social.msdn.microsoft.com/Search/en-US?query=%e3%81%82%e3%81%b0%e3%81%b0%e3%81%b0%ef%bd%82%e3%81%98%e3%81%88%e3%81%88%ef%bd%97%e3%82%8c%ef%bd%97&beta=0&ac=8";
		LPSTR penc = UrlEncode((LPBYTE)p);
		stlsoft::scoped_handle<void*> mapenc(penc, free);
		
		LPBYTE pdec = UrlDecode(penc);
		stlsoft::scoped_handle<void*> mapdec(pdec, free);
		assert(strcmp(p, (LPCSTR)pdec) == 0);

		//LPCWSTR wp = L"https://social.msdn.microsoft.com/Search/en-US?query=%e3%81%82%e3%81%b0%e3%81%b0%e3%81%b0%ef%bd%82%e3%81%98%e3%81%88%e3%81%88%ef%bd%97%e3%82%8c%ef%bd%97&beta=0&ac=8";
		//LPWSTR wpenc = UrlEncodeW(wp);
		//LPWSTR wpdec = UrlDecodeW(wpenc);
		//assert(wcscmp(wp, wpdec) == 0);
	}

	{
		//LPCSTR p = "あいうえお";
		//LPSTR penc = UrlEncode((BYTE*)p);
		//LPBYTE pdec = UrlDecode(penc);
		//assert(strcmp(p, (LPCSTR)pdec) == 0);

		LPCWSTR wp = L"あいうえお";
		std::wstring wenc = UrlEncodeW(wp);
		std::wstring wdec = UrlDecodeW(wenc.c_str());
		assert(wcscmp(wp, wdec.c_str()) == 0);
	}

}