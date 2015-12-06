// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "SessionGlobalMemory.h"
using namespace Ambiesoft;

struct Data {
	int d[3];
	Data(const Data& data) {
		memcpy(d, &data.d, sizeof(d));
		ctorcount++;
	}
	Data(const Data&& data) {
		memcpy(d, &data.d, sizeof(d));
		ctorcount++;
	}

	Data() {
		memset(d,0,sizeof(d));
		ctorcount++;
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

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	CSessionGlobalMemory<Data> sgData("sgData");
	CSessionGlobalMemoryNTS<Data> sgDataNTS("sgData");

	CSessionGlobalMemory<Data> tmp(sgData);
	CSessionGlobalMemory<int> tmp2 = getSGMEM(3, "sgINT");
	CSessionGlobalMemory<int> tmp3 = getSGMEM(3, "sgINTtmp");
	tmp3=tmp2;
	tmp2=std::move(tmp3);

	bool quit=false;
	for(; !quit;)
	{
		printf("\nPush 'a' to assign data, 'r' to read data and 'q' to quit.\n");
		switch(_getch())
		{
			case 'a':
			{
				Data data;
				data.d[0]=rand();
				data.d[1]=rand();
				data.d[2]=rand();
				sgData = data;
				printf("Data assigned:\t%d, %d, %d\n", data.d[0], data.d[1], data.d[2]);
			}
			break;
			case 'A':
			{
				Data data;
				data.d[0]=rand();
				data.d[1]=rand();
				data.d[2]=rand();
				memcpy(&sgDataNTS,&data,sizeof(data));
				printf("Data assigned:\t%d, %d, %d\n", data.d[0], data.d[1], data.d[2]);
			}
			break;

			case 'r':
			{
				Data::print();
				Data data = sgData;
				Data::print();

				//data = sgData;
				printf("Data Read:\t%d, %d, %d\n", data.d[0], data.d[1], data.d[2]);
			}
			break;
			case 'R':
			{
				Data data;
				memcpy(&data, &sgDataNTS, sizeof(data));
				printf("Data Read:\t%d, %d, %d\n", data.d[0], data.d[1], data.d[2]);
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

