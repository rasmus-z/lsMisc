#include "stdafx.h"
#include <tchar.h>
#include <vector>
#include <string>

#include "../SHMoveFile.h"

using namespace std;
using namespace Ambiesoft;

void testSHMoveFile()
{
	LPCWSTR p1 = L"C:\\Users\\Bokkurin\\Desktop\\Network Emulator Toolkit x32.rar";
	LPCWSTR p2 = L"C:\\Users\\Bokkurin\\Desktop\\Network Emulator Toolkit x64.rar";
	LPCWSTR pT = L"\\\\Thexp\\Share";


	//SHMoveOneFile(pT, p1);
	//SHMoveOneFile(pT, p2);

	vector<wstring> v;
	v.push_back(p1);
	v.push_back(p2);

	int nRet;
	SHMoveFile(pT, v, &nRet);
}