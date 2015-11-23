#include "stdafx.h"
#include <AFX.h>
#include "LCMapCommon.h"

static CString LCMapCommon(LPCWSTR p, DWORD flag)
{
	size_t len = lstrlenW(p);

	size_t outlen = (len+1)*2;
	WCHAR* q = (WCHAR*)malloc(outlen*sizeof(WCHAR));
	struct Qfree {
		WCHAR* q_;
		Qfree(WCHAR* q) {q_=q;}
		~Qfree(){free(q_);}
	} qfree(q);

	int resultlen=LCMapStringW(
				LOCALE_SYSTEM_DEFAULT,
				flag,
				p,
				len,
				q,
				outlen);
	if(resultlen==0)
	{
		return L"";
	}
	q[resultlen]=0;
	return CString(q,resultlen);
}

CString Han2Zen(LPCWSTR p)
{
	return LCMapCommon(p, LCMAP_FULLWIDTH);
}
CString Zen2Han(LPCWSTR p)
{
	return LCMapCommon(p, LCMAP_HALFWIDTH);
}

CString Kata2Hira(LPCWSTR p)
{
	return LCMapCommon(p, LCMAP_HIRAGANA);
}
CString Hira2Kata(LPCWSTR p)
{
	return LCMapCommon(p, LCMAP_KATAKANA);
}

