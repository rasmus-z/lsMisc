#include "stdafx.h"

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <cassert>
#include <string>
#include <ostream>
#include <fstream>

#include "../stdwin32/stdwin32.h"
using namespace stdwin32;

#include "../UTF16toUTF8.h"
#include "../I18N.h"
using namespace Ambiesoft;

using namespace std;


static void prepareLang()
{
	wstring langdir = stdCombinePath(stdGetParentDirectory(stdGetModuleFileName()), L"lang");
	CreateDirectory(langdir.c_str(), NULL);

	wstring langfile = stdCombinePath(langdir, L"jpn.txt");
	if (PathFileExists(langfile.c_str()))
		return;


	wstring data(L"\"AAA\"=\"あああ\"");
	BYTE* pUtf8 = UTF16toUTF8(data.c_str());

	FILE* f = NULL;
	_wfopen_s(&f, langfile.c_str(), L"wb");
	assert(f);
	fwrite(pUtf8, strlen((char*)pUtf8),1, f);
	fclose(f);
}

void testI18N()
{
	prepareLang();
	i18nInitLangmap(NULL, L"JPN");

	// wchar_t aaa[] = { 0x4230, 0x4230, 0x4230, 0 };
	wchar_t aaa[] = { 0x3042, 0x3042, 0x3042, 0 };
	assert(std::wstring(I18N(L"AAA")) == aaa);
}