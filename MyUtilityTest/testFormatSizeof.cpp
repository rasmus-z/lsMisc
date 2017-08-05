#include "stdafx.h"
#include <string>

#include "../FormatSizeof.h"

void testFormatSizeof()
{
	long long ll;
	ll = 10;
	std::wstring ws = FormatSizeof(ll);
	mytrace(ws);
	

	mytrace(FormatSizeof(1233LL));
	mytrace(FormatSizeof(334643131LL));
	mytrace(FormatSizeof(13778971131LL));
	mytrace(FormatSizeof(79741314LL));
	mytrace(FormatSizeof(123456777666LL));
	mytrace(FormatSizeof(4676464874646464465LL));
}