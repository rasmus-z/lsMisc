#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include "ProcessList.h"

BOOL GetProcessList(std::vector<PROCESSENTRY32>& ret)
{
	HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
		return FALSE;

	PROCESSENTRY32 pe;
	int httpdcount = 0;
	int mysqldcount = 0;
	pe.dwSize=sizeof(PROCESSENTRY32);
	BOOL retval=Process32First(hSnapshot,&pe);
	while(retval)
	{
		ret.push_back(pe);
		retval=Process32Next(hSnapshot,&pe);
	}

	CloseHandle(hSnapshot);
	return TRUE;
}