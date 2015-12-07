// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <process.h>
#include "SessionGlobalMemory.h"
using namespace Ambiesoft;

struct Data {
	int d[1024];
	int checksum_;

	Data() {
		memset(d, 0, sizeof(d));
		checksum_ = 0;
		ctorcount++;
	}

	Data(const Data& data) {
		memcpy(d, &data.d, sizeof(d));
		checksum_ = data.checksum_;
		ctorcount++;
	}



	void setrand()
	{
		checksum_ = 0;
		for (int i = 0; i < _countof(d); ++i)
		{
			d[i] = rand();
			checksum_ += d[i];
		}
	}

	bool isChesksumOK()
	{
		int cs = 0;
		for (int i = 0; i < _countof(d); ++i)
		{
			cs += d[i];
		}
		assert(cs == checksum_);
		return checksum_ == cs;
	}
	static int ctorcount;
	static void print() {
		printf("ctorcount = %d\n", ctorcount);
	}
};
int Data::ctorcount;


template<class T>
CSessionGlobalMemory<T> getSGMEM(T initialvalue, LPCTSTR pName)
{
	CSessionGlobalMemory<T> ret(pName);
	ret = initialvalue;
	return ret;
}

void testreader(int span)
{
	CSessionGlobalMemory<Data> sgData("sgData");
	for (;;)
	{
		Data data;
		sgData.get(data);
		printf("Data Read:\t%d, %d, %d, ...\n", data.d[0], data.d[1], data.d[2]);
		printf("Checksum is %s.\n", data.isChesksumOK() ? "OK" : "NG");

		Sleep(span);
	}
}

void testwriter(int span)
{
	CSessionGlobalMemory<Data> sgData("sgData");
	for (;;)
	{
		Data data;
		data.setrand();
		sgData = data;
		printf("Data assigned:\t%d, %d, %d, ...\n", data.d[0], data.d[1], data.d[2]);
		printf("Checksum is %s.\n", data.isChesksumOK() ? "OK" : "NG");

		Sleep(span);
	}
}

int main(int argc, char* argv[])
{

	// test.exe -reader 100
	if (argc > 1)
	{
		
		int i= 1;
		// puts(argv[i]);
		if (_strcmpi(argv[i], "-reader") == 0)
		{
			testreader(atoi(argv[i + 1]));
			return 0;
		}
		else if (_strcmpi(argv[i], "-writer") == 0)
		{
			testwriter(atoi(argv[i + 1]));
			return 0;
		}
		else
		{
			assert(false);
			return -1;
		}
	}

	srand((unsigned int)time(NULL));
	CSessionGlobalMemory<Data> sgData("sgData");
	CSessionGlobalMemoryNTS<Data> sgDataNTS("sgData");

	//CSessionGlobalMemory<Data> tmp(sgData);
	CSessionGlobalMemory<int> tmp2 (getSGMEM(3, "sgINT"));
	CSessionGlobalMemory<int> tmp3 (getSGMEM(3, "sgINTtmp"));
	tmp3=tmp2;
#if !defined(AMBIESOFT_NO_RVALUE_)
	tmp2=std::move(tmp3);
#endif
	std::string sa = "aaa";
	std::string sb;
#if !defined(AMBIESOFT_NO_RVALUE_)
	sb = std::move(sa);
#endif
	sb = "bbb";
	bool quit=false;
	for(; !quit;)
	{
		printf("\nPush 'a' to assign data, 'r' to read data, '0' to launch reader, '1' to launch writer,  and 'q' to quit.\n");
		switch(int ch=_getch())
		{
			case '0': case '1':
			{
				// http://www.cplusplus.com/forum/beginner/103383/
				char* comspec = getenv("COMSPEC");
				_spawnl(_P_NOWAIT // flags
					, comspec   // cmd line
					, comspec   // arg[0] to main (of target)
					, "/c"      // arg[1]
					, "start"   // etc (see note above)
					, comspec
					, "/c"
					, argv[0]
					, ch == '0' ? "-reader" : "-writer"
					, "100"
					, NULL);
			}
			break;
			
			case 'a':
			{
				Data data;
				data.setrand();
				sgData = data;
				printf("Data assigned:\t%d, %d, %d, ...\n", data.d[0], data.d[1], data.d[2]);
				printf("Checksum is %s.\n", data.isChesksumOK() ? "OK":"NG");
			}
			break;
			case 'A':
			{
				Data data;
				data.d[0]=rand();
				data.d[1]=rand();
				data.d[2]=rand();
				memcpy(&sgDataNTS,&data,sizeof(data));
				printf("Data assigned:\t%d, %d, %d, ...\n", data.d[0], data.d[1], data.d[2]);
				printf("Checksum is %s.\n", data.isChesksumOK() ? "OK" : "NG");
			}
			break;

			case 'r':
			{
				Data data;
				sgData.get(data);
				
				printf("Data Read:\t%d, %d, %d, ...\n", data.d[0], data.d[1], data.d[2]);
				printf("Checksum is %s.\n", data.isChesksumOK() ? "OK" : "NG");
			}
			break;
			case 'R':
			{
				Data data;
				memcpy(&data, &sgDataNTS, sizeof(data));
				printf("Data Read:\t%d, %d, %d, ...\n", data.d[0], data.d[1], data.d[2]);
				printf("Checksum is %s.\n", data.isChesksumOK() ? "OK" : "NG");
			}
			break;


			case 'q':
			case 'Q':
			{
				quit = true;
				break;
			}
			break;

			default:
				break;
		}
	}


	return 0;
}

