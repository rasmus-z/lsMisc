#include "stdafx.h"
//#include <AFX.h>
#include "tstring.h"
#include <windows.h>
#include <tchar.h>
#include "LCMapCommon.h"

static tstring LCMapCommon(LPCTSTR p, DWORD flag)
{
	size_t len = lstrlen(p);

	size_t outlen = (len+1)*2;
	TCHAR* q = (TCHAR*)malloc(outlen*sizeof(TCHAR));
	struct Qfree {
		TCHAR* q_;
		Qfree(TCHAR* q) {q_=q;}
		~Qfree(){free(q_);}
	} qfree(q);

	int resultlen=LCMapString(
				LOCALE_SYSTEM_DEFAULT,
				flag,
				p,
				len,
				q,
				outlen);
	if(resultlen==0)
	{
		return _T("");
	}
	q[resultlen]=0;
	return tstring(q,resultlen);
}

tstring Han2Zen(LPCTSTR p)
{
	return LCMapCommon(p, LCMAP_FULLWIDTH);
}
tstring Zen2Han(LPCTSTR p)
{
	return LCMapCommon(p, LCMAP_HALFWIDTH);
}

tstring Kata2Hira(LPCTSTR p)
{
	return LCMapCommon(p, LCMAP_HIRAGANA);
}
tstring Hira2Kata(LPCTSTR p)
{
	return LCMapCommon(p, LCMAP_KATAKANA);
}

