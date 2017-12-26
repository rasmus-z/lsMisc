#include "stdafx.h"
#include <Windows.h>


#include <cassert>
#include <string>

#include "../ExpandPath.h"

#include "debug.h"

using namespace Ambiesoft;
using namespace std;

void testExpandPath()
{
	vector<wstring> results;

	results.clear();
	ExpandPath(L"C:\\T\\*\\abc\\*", results);

	results.clear();
	ExpandPath(L"C:\\T\\*\\abc", results);

	results.clear();
	ExpandPath(L"C:\\T\\*\\abc\\", results);

	results.clear();
	ExpandPath(L"C:\\T\\*\\", results);

	results.clear();
	ExpandPath(L"C:\\T\\*", results);

}