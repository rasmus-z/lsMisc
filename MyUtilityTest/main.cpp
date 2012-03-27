#include <windows.h>
#include <tchar.h>

#include <string>

#include "../tstring.h"
#include "../ChangeFilenamable2.h"
#include "../IsFileExists.h"

int main()
{
	TCHAR szT[] = "äJî≠ã@ç\ì¡ï éëóø";
	tstring t = ChangeFilenamable2(szT);

	BOOL b = IsFileExists("c:\\T\\aaa.txt");
	b = IsFileExists("c:\\T\\bbb.txt");
	b = IsFileExists("c:\\T\\ccc");
	return 0;
}
