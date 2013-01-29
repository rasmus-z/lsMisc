#include <windows.h>
#include <tchar.h>
#include <TlHelp32.h>

#include <string>
#include <vector>


#include "../tstring.h"
#include "../ChangeFilenamable2.h"
#include "../IsFileExists.h"
#include "../ProcessList.h"

#pragma comment(lib,"Shell32.lib")
#pragma comment(lib,"comsuppw.lib")
#pragma comment(lib,"Advapi32.lib")


int main()
{
	std::vector<PROCESSENTRY32> ret;
	GetProcessList(ret);

	TCHAR szT[] = "äJî≠ã@ç\ì¡ï éëóø";
	tstring t = ChangeFilenamable2(szT);

	BOOL b = IsFileExists("c:\\T\\aaa.txt");
	b = IsFileExists("c:\\T\\bbb.txt");
	b = IsFileExists("c:\\T\\ccc");
	return 0;
}
