#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>

#include <cassert>
#include <string.h>
#include <string>

#include "../PathUtil.h"

#include "debug.h"

using namespace Ambiesoft;

void testPathUtil()
{
	LPCTSTR p1, p2;
	std::wstring duppath;

	DVERIFY(myPathIsRoot(L"C:"));
	DVERIFY(myPathIsRoot(L"C:\\"));
	DVERIFY(!myPathIsRoot(L"C:\\a"));
	DVERIFY(myPathIsRoot(L"\\\\"));
	DVERIFY(myPathIsRoot(L"\\\\thexp"));
	DVERIFY(!myPathIsRoot(L"\\\\thexp\\a"));
	DVERIFY(!myPathIsRoot(L"\\\\thexp\\a\\b"));
	DVERIFY(!myPathIsRoot(L""));
	DVERIFY(!myPathIsRoot(L".."));
	DVERIFY(myPathIsRoot(L"../../../../../../../../../../../../../../../../../../../../../"));

	p1 = L"C:\\a\\b\\c";
	p2 = L"C:\\a\\b/d";
	DVERIFY(!myPathIsChildIncluded(p1, p2));

	p1 = L"C:\\a\\b\\c/../..";
	p2 = L"C:\\a\\b/d";
	DVERIFY(myPathIsChildIncluded(p1, p2, &duppath));
	DVERIFY(lstrcmpi(duppath.c_str(), L"c:\\a") == 0);

	p1 = L"\\\\thexp\\share\\";
	p2 = L"\\\\THEXP\\share\\b";
	DVERIFY(myPathIsChildIncluded(p1, p2, &duppath));
	DVERIFY(lstrcmpi(duppath.c_str(), L"\\\\thexp\\share") == 0);

	p1 = L"\\\\thexp\\share\\xxx";
	p2 = L"\\\\THEXP\\share\\b";
	DVERIFY(!myPathIsChildIncluded(p1, p2, &duppath));

	p1 = L"\\\\thexp\\share\\aaa";
	p2 = L"\\\\THEXP\\share\\aaa\\vvvsdf\\fwefew";
	DVERIFY(myPathIsChildIncluded(p1, p2, &duppath));
	DVERIFY(lstrcmpi(duppath.c_str(), L"\\\\thexp\\share\\aaa") == 0);


	p1 = L"c:\\a\\b\\x\\f";
	p2 = L"c:\\a\\b/y";
	DVERIFY(!myPathIsChildIncluded(p1, p2, &duppath));

	p1 = L"c:\\t";
	p2 = L"c:\\";
	DVERIFY(myPathIsChildIncluded(p1, p2, &duppath));
}