#include <windows.h>
#include <tchar.h>

#include <string>

#include "../tstring.h"
#include "../ChangeFilenamable2.h"


int main()
{
	TCHAR szT[] = "ŠJ”­‹@\“Á•Ê‘—¿";
	tstring t = ChangeFilenamable2(szT);
	return 0;
}
