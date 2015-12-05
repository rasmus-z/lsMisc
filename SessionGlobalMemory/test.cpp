// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "SessionGlobalMemory.h"
using namespace Ambiesoft;

struct Data {
	int d[3];
	Data(const Data& data) {
		memcpy(this, &data, sizeof(this));
		ctorcount++;
	}

	Data() {
		memset(d,0,sizeof(this));
		ctorcount++;
	}

	static int ctorcount;
	static void print() {
		printf("ctorcount = %d\n", ctorcount);
	}
};
int Data::ctorcount;

int main(int argc, char* argv[])
{
	srand(time(NULL));
	CSessionGlobalMemory<Data> sgData("sgData");
	CSessionGlobalMemoryNTS<Data> sgDataNTS("sgData");

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

				data = sgData;
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

