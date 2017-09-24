#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <TlHelp32.h>

#include <string>
#include <vector>

#pragma comment(lib,"Shell32.lib")
// #pragma comment(lib,"comsuppw.lib")
#pragma comment(lib,"Advapi32.lib")



#include "../tstring.h"
#include "../IsFileExists.h"
#include "../ProcessList.h"
#include "../ChangeFilenamable.h"
#include "../PathUtil.h"
#include "../OpenedFiles.h"

#include <stlsoft/smartptr/scoped_handle.hpp>


// あああ 日本語

void testMoveWindowCommon();
void testCommandLine();
void testHira2Kata();
void testGetVersionString();
void testStlMutex();
void testPathUtil();
void testUrlEncode();
void testFormatSizeof();
void testSHMoveFile();
void testOpenCommon();
void testOpenedFiles();
void testI18N();
void testCommandLineString();
void testOSTraits();

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(
		_CRTDBG_ALLOC_MEM_DF		|
		_CRTDBG_LEAK_CHECK_DF		| 
		_CRTDBG_CHECK_ALWAYS_DF		|
		_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif
	testOSTraits();
	testCommandLineString();
	testSHMoveFile();
	testOpenedFiles();
	
	
	testCommandLine();
	testI18N();
	
	testOpenCommon();

	testFormatSizeof();
	testUrlEncode();
	testPathUtil();

	testMoveWindowCommon();
	
	testHira2Kata();
	testGetVersionString();






	// kernel object
	{
		HANDLE h = CreateMutex(NULL, FALSE, NULL);
		if (h == NULL)
			return -1;

		stlsoft::scoped_handle<HANDLE> ko(h, CloseHandle);
	}

	{
		FILE* f = NULL;
#if _MSC_VER <= 1200
		f = fopen("C:\\T\\Test.txt", "w");
#else
		fopen_s(&f, "C:\\T\\Test.txt", "w");
#endif
		if (!f)
			return -1;

		stlsoft::scoped_handle<FILE*> fo(f, fclose);
	}

	//TCHAR buf[MAX_PATH];




	std::vector<PROCESSENTRY32> ret;
	GetProcessList(ret);

	TCHAR szT[] = _T("開発機構特別資料");
	tstring t = ChangeFilenamable(szT);

	BOOL b = IsFileExists(_T("c:\\T\\aaa.txt"));
	b = IsFileExistsA("c:\\T\\bbb.txt");
	b = IsFileExistsW(L"c:\\T\\ccc");
	return 0;
}
