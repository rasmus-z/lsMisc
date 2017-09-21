#include "stdafx.h"
#include <tchar.h>
#include <vector>
#include <string>

#include "../SHMoveFile.h"

#include "debug.h"

using namespace std;
using namespace Ambiesoft;

void testSHMoveFile()
{
	LPCWSTR p1 = L"C:\\T\\Old.txt";
	LPCWSTR p2 = L"C:\\T\\New.txt";
	DVERIFY(SHMoveFile(p2, p1));
	DVERIFY(SHMoveFile(p1, p2));

	LPCWSTR p3 = L"C:\\T\\New1.txt";
	LPCWSTR p4 = L"C:\\T\\New2.txt";
	DVERIFY(SHCopyFile(p3, p1));
	DVERIFY(SHCopyFile(p4, p1));

	LPCWSTR dir = L"C:\\T\\DIR";
	CreateDirectory(dir, NULL);
	vector<wstring> ws;
	ws.push_back(p3);
	ws.push_back(p4);
	DVERIFY(SHMoveFile(dir, ws));

	ws.clear();
	dir = L"C:\\T\\";
	p3 = L"C:\\T\\DIR\\New1.txt";
	p4 = L"C:\\T\\DIR\\New2.txt";
	ws.push_back(p3);
	ws.push_back(p4);
	DVERIFY(SHMoveFile(dir, ws));
	
	ws.clear();
	dir = L"C:\\T\\DIR";
	p3 = L"C:\\T\\New1.txt";
	p4 = L"C:\\T\\New2.txt";
	ws.push_back(p3);
	ws.push_back(p4);
	DVERIFY(SHCopyFile(dir, ws));


	p3 = L"C:\\T\\New1.txt";
	p4 = L"C:\\T\\New2.txt";
	LPCWSTR p5 = L"C:\\T\\New5.txt";
	LPCWSTR p6 = L"C:\\T\\New6.txt";
	
	vector<wstring> wsS;
	vector<wstring> wsD;
	wsS.push_back(p3);
	wsS.push_back(p4);
	wsD.push_back(p5);
	wsD.push_back(p6);
	DVERIFY(SHMoveFile(wsD, wsS));

	wsS.clear();
	wsD.clear();
	wsS.push_back(p5);
	wsS.push_back(p6);
	wsD.push_back(p3);
	wsD.push_back(p4);
	DVERIFY(SHCopyFile(wsD, wsS));
}