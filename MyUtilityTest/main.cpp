#include <windows.h>
#include <tchar.h>
#include <TlHelp32.h>

#include <string>
#include <vector>

#pragma comment(lib,"Shell32.lib")
#pragma comment(lib,"comsuppw.lib")
#pragma comment(lib,"Advapi32.lib")



#include "../tstring.h"
#include "../IsFileExists.h"
#include "../ProcessList.h"
#include "../ChangeFilenamable.h"
#include "../GetVersionString.h"



int main()
{
	TCHAR buf[MAX_PATH];

	// Get file description for explorer.exe
	if(GetVersionString("C:\\windows\\explorer.exe", "FileDescription", buf, MAX_PATH))
		printf("%s\n", buf);


	std::vector<PROCESSENTRY32> ret;
	GetProcessList(ret);

	TCHAR szT[] = "äJî≠ã@ç\ì¡ï éëóø";
	tstring t = ChangeFilenamable(szT);

	BOOL b = IsFileExists("c:\\T\\aaa.txt");
	b = IsFileExists("c:\\T\\bbb.txt");
	b = IsFileExists("c:\\T\\ccc");
	return 0;
}
