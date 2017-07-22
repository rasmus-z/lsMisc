#include "stdafx.h"
//#include <afx.h>
#include <windows.h>
#include <string>
#include <iostream>
#include "../LCMapCommon.h"

using namespace std;

void testHira2Kata()
{
	std::wstring as(L"aaaa", 4);

	LPCWSTR p=L"あああアアア ｱｱｱ";
	tstring s = Kata2Hira(p);
	OutputDebugString(s.c_str());
	OutputDebugString(L"\r\n");


	s=Hira2Kata(p);
	OutputDebugString(s.c_str());
	OutputDebugString(L"\r\n");
	
}