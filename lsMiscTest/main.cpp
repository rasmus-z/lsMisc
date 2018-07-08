#include "stdafx.h"



#include <windows.h>
#include <tchar.h>
#include <TlHelp32.h>

#include <string>
#include <vector>

#pragma comment(lib,"Shell32.lib")
#pragma comment(lib,"Advapi32.lib")

#if _MSC_VER > 1200
#include "gtest/gtest.h"
#endif

#include "../tstring.h"
#include "../IsFileExists.h"
#include "../ChangeFilenamable.h"
#include "../PathUtil.h"
#include "../OpenedFiles.h"

#include <stlsoft/smartptr/scoped_handle.hpp>






void testStlMutex();
void testOpenedFiles();
void testExpandPath();

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(
		_CRTDBG_ALLOC_MEM_DF		|
		_CRTDBG_LEAK_CHECK_DF		| 
		_CRTDBG_CHECK_ALWAYS_DF		|
		_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif

	testing::InitGoogleTest(&__argc, __argv);
	int gret = RUN_ALL_TESTS();

	
	//testExpandPath();
	//testOpenedFiles();
	//testStlMutex();



	BOOL b = IsFileExists(_T("c:\\T\\aaa.txt"));
	b = IsFileExistsA("c:\\T\\bbb.txt");
	b = IsFileExistsW(L"c:\\T\\ccc");
	return 0;
}

