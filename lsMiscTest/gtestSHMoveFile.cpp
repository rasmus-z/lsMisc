#include "stdafx.h"
#include <tchar.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>


#include "gtest/gtest.h"

#include "../SHMoveFile.h"

#include "debug.h"

#include "../DebugNew.h"

using namespace std;
using namespace Ambiesoft;

#pragma comment(lib,"Rpcrt4.lib")

static wstring uuidgen()
{
	UUID uuid;
	UuidCreate(&uuid);
	RPC_WSTR p;
	UuidToStringW(&uuid,&p);
	wstring ret((wchar_t*)p);
	RpcStringFree(&p);
	return ret;
}

class FileOperationTest : public ::testing::Test
{
protected:
	wstring mystring;
	FileOperationTest()
	{
		mystring = L"aaa";
	}
	virtual void SetUp()
	{
		
	}
	virtual void TearDown()
	{

	}
};
TEST_F(FileOperationTest, MoveFile) {
	ASSERT_TRUE(mystring == L"aaa");

	wstring s1 = L"C:\\T\\";
	s1 += uuidgen();
	wofstream myfile;
	myfile.open(s1.c_str());
	myfile << L"aaa";
	myfile.close();
	LPCWSTR p1 = s1.c_str();
	LPCWSTR p2 = L"C:\\T\\New.txt";
	EXPECT_EQ(SHMoveFile(p2, p1), 0);
	EXPECT_EQ(SHMoveFile(p1, p2), 0);

	LPCWSTR p3 = L"C:\\T\\New1.txt";
	LPCWSTR p4 = L"C:\\T\\New2.txt";
	EXPECT_TRUE(SHCopyFile(p3, p1, FOF_NO_UI) == 0);
	EXPECT_TRUE(SHCopyFile(p4, p1, FOF_NO_UI) == 0);

	LPCWSTR dir = L"C:\\T\\DIR";
	CreateDirectory(dir, NULL);
	vector<wstring> ws;
	ws.push_back(p3);
	ws.push_back(p4);
	EXPECT_TRUE(SHMoveFile(dir, ws, FOF_NO_UI) == 0);

	ws.clear();
	dir = L"C:\\T\\";
	p3 = L"C:\\T\\DIR\\New1.txt";
	p4 = L"C:\\T\\DIR\\New2.txt";
	ws.push_back(p3);
	ws.push_back(p4);
	EXPECT_TRUE(SHMoveFile(dir, ws, FOF_NO_UI) == 0);

	ws.clear();
	dir = L"C:\\T\\DIR";
	p3 = L"C:\\T\\New1.txt";
	p4 = L"C:\\T\\New2.txt";
	ws.push_back(p3);
	ws.push_back(p4);
	EXPECT_TRUE(SHCopyFile(dir, ws, FOF_NO_UI) == 0);


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
	EXPECT_TRUE(SHMoveFile(wsD, wsS, FOF_NO_UI) == 0);

	wsS.clear();
	wsD.clear();
	wsS.push_back(p5);
	wsS.push_back(p6);
	wsD.push_back(p3);
	wsD.push_back(p4);
	EXPECT_TRUE(SHCopyFile(wsD, wsS, FOF_NO_UI) == 0);
}
TEST_F(FileOperationTest, CopyFile) {
	EXPECT_TRUE(true);
}
TEST_F(FileOperationTest, DeleteFile) {
	EXPECT_TRUE(true);
}
//void testSHMoveFile()
//{
//	wstring s1 = L"C:\\T\\";
//	s1+=uuidgen();
//	wofstream myfile;
//	myfile.open(s1.c_str());
//	myfile << L"aaa";
//	myfile.close();
//	LPCWSTR p1=s1.c_str();
//	LPCWSTR p2 = L"C:\\T\\New.txt";
//	DVERIFY(SHMoveFile(p2, p1)==0);
//	DVERIFY(SHMoveFile(p1, p2)==0);
//
//	LPCWSTR p3 = L"C:\\T\\New1.txt";
//	LPCWSTR p4 = L"C:\\T\\New2.txt";
//	DVERIFY(SHCopyFile(p3, p1)==0);
//	DVERIFY(SHCopyFile(p4, p1)==0);
//
//	LPCWSTR dir = L"C:\\T\\DIR";
//	CreateDirectory(dir, NULL);
//	vector<wstring> ws;
//	ws.push_back(p3);
//	ws.push_back(p4);
//	DVERIFY(SHMoveFile(dir, ws)==0);
//
//	ws.clear();
//	dir = L"C:\\T\\";
//	p3 = L"C:\\T\\DIR\\New1.txt";
//	p4 = L"C:\\T\\DIR\\New2.txt";
//	ws.push_back(p3);
//	ws.push_back(p4);
//	DVERIFY(SHMoveFile(dir, ws)==0);
//	
//	ws.clear();
//	dir = L"C:\\T\\DIR";
//	p3 = L"C:\\T\\New1.txt";
//	p4 = L"C:\\T\\New2.txt";
//	ws.push_back(p3);
//	ws.push_back(p4);
//	DVERIFY(SHCopyFile(dir, ws)==0);
//
//
//	p3 = L"C:\\T\\New1.txt";
//	p4 = L"C:\\T\\New2.txt";
//	LPCWSTR p5 = L"C:\\T\\New5.txt";
//	LPCWSTR p6 = L"C:\\T\\New6.txt";
//	
//	vector<wstring> wsS;
//	vector<wstring> wsD;
//	wsS.push_back(p3);
//	wsS.push_back(p4);
//	wsD.push_back(p5);
//	wsD.push_back(p6);
//	DVERIFY(SHMoveFile(wsD, wsS)==0);
//
//	wsS.clear();
//	wsD.clear();
//	wsS.push_back(p5);
//	wsS.push_back(p6);
//	wsD.push_back(p3);
//	wsD.push_back(p4);
//	DVERIFY(SHCopyFile(wsD, wsS)==0);
//}