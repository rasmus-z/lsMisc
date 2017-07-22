#include "stdafx.h"
#include <cassert>

#include "../stdwin32.h"

using namespace std;
using namespace stdwin32;

int main(int argc, char* argv[])
{
	std::string s=	stdwin32::string_format("%d-%s", 100, "aaa");
	wstring ws;

	s = stdItoA(1234);
	assert(s=="1234");

	s = stdItoA(-3334);
	assert(s=="-3334");


	s = stdItoA64(4444555522233444L);
	assert(s=="4444555522233444");

	ws = stdItoW64(-1010);
	assert(ws==L"-1010");
	
	ws = stdItoW(5);
	assert(ws==L"5");

	return 0;
}
