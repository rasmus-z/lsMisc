#include "stdafx.h"

#include <cassert>
#include <string>

#include "../I18N.h"
using namespace Ambiesoft;

void testI18N()
{
	i18nInitLangmap();

	assert(std::wstring(I18N(L"AAA")) == L"‚ ‚ ‚ ");
}