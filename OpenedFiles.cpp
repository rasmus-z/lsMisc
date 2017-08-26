#include "StdAfx.h"
#include <Tlhelp32.h>
#include <Psapi.h>
#include <Shlwapi.h>

#include <string>
#include <algorithm>
#include <sstream>

#include "OpenedFiles.h"

#pragma comment(lib,"shlwapi.lib")

using namespace std;

#pragma warning(disable:4996)

#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#define IOCTL_LISTDRV_BUFFERED_IO		\
        CTL_CODE(FILE_DEVICE_UNKNOWN,	\
        0x802,							\
        METHOD_BUFFERED,	            \
        FILE_READ_DATA | FILE_WRITE_DATA)

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemHandleInformation = 0X10,
} SYSTEM_INFORMATION_CLASS;

typedef struct _HANDLE_INFO
{
	USHORT tcDeviceName[260];
	USHORT tcFileName[260];
	ULONG uType;
}HANDLE_INFO;

typedef struct _ADDRESS_INFO
{
	PVOID pAddress;
}ADDRESS_INFO;

typedef DWORD(WINAPI* GetFinalPathNameByHandleDef)(
	HANDLE hFile,
	LPWSTR lpszFilePath,
	DWORD cchFilePath,
	DWORD dwFlags);

typedef NTSTATUS(WINAPI *PNtQuerySystemInformation)
(IN	SYSTEM_INFORMATION_CLASS SystemInformationClass,
OUT	PVOID					 SystemInformation,
IN	ULONG					 SystemInformationLength,
OUT	PULONG					 ReturnLength OPTIONAL);

void EnumerateLoadedModules(
	wstring& csPath,
	OF_CALLBACK CallBackProc,
	UINT_PTR pUserContext);

void EnumerateOpenedFiles(
	wstring& csPath,
	OF_CALLBACK CallBackProc,
	UINT_PTR pUserContext,
	HANDLE hDriver,
	GetFinalPathNameByHandleDef pGetFinalPathNameByHandle);

enum {
	VISTA_FILETYPE = 25,
	XP_FILETYPE = 28,
};
typedef struct _SYSTEM_HANDLE
{
	DWORD       dwProcessId;
	BYTE		bObjectType;
	BYTE		bFlags;
	WORD		wValue;
	PVOID       pAddress;
	DWORD GrantedAccess;
}SYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
	DWORD         dwCount;
	SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION, **PPSYSTEM_HANDLE_INFORMATION;

struct PROCESS_INFO_t
{
	wstring csProcess;
	DWORD dwImageListIndex;
};

static void MakeLower(wstring& s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::towlower);
}
static BOOL EnableTokenPrivilege(LPCTSTR pszPrivilege)
{
	// do it only once
	static bool bEnabled = false;
	if (bEnabled)
	{
		return TRUE;
	}
	bEnabled = true;

	HANDLE hToken = 0;
	TOKEN_PRIVILEGES tkp = { 0 };

	// Get a token for this process.
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES |
		TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}

	// Get the LUID for the privilege. 
	if (LookupPrivilegeValue(NULL, pszPrivilege,
		&tkp.Privileges[0].Luid))
	{
		tkp.PrivilegeCount = 1;  // one privilege to set    
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		// Set the privilege for this process. 
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
			(PTOKEN_PRIVILEGES)NULL, 0);

		if (GetLastError() != ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

	return FALSE;
}

static bool replace(std::wstring& str, const std::wstring& from, const std::wstring& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::wstring::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

// Function resolves the fosedevice name to drive name
static BOOL GetDrive(LPCTSTR pszDosName, wstring& csDrive, bool bDriveLetterOnly)
{
	TCHAR tcDeviceName[50];
	TCHAR tcDrive[3] = _T("A:");

	// Iterating through the drive letters
	for (TCHAR actDrive = _T('A'); actDrive <= _T('Z'); actDrive++)
	{
		tcDrive[0] = actDrive;
		// Query the device for the drive letter
		if (QueryDosDevice(tcDrive, tcDeviceName, 50) != 0)
		{
			// Is this the drive letter we are looking for?
			if (_tcsnicmp(tcDeviceName, pszDosName, _tcslen(tcDeviceName)) == 0)
			{
				if (bDriveLetterOnly)
				{
					csDrive = tcDrive;
				}
				else
				{
					csDrive = pszDosName;
					// csDrive.Replace(tcDeviceName, tcDrive);
					replace(csDrive, tcDeviceName, tcDrive);
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

void GetOpenedFiles(LPCWSTR lpPath, OF_TYPE Filter, OF_CALLBACK CallBackProc,
	UINT_PTR pUserContext)
{
	wstring csPath = lpPath;
	std::transform(csPath.begin(), csPath.end(), csPath.begin(), ::towlower);


	EnableTokenPrivilege(SE_DEBUG_NAME);
	if (Filter& MODULES_ONLY)
	{
		EnumerateLoadedModules(csPath, CallBackProc, pUserContext);
	}

	if (Filter& FILES_ONLY)
	{
		// Extract the driver from the resource and install it.
		// HANDLE hDriver = ExtractAndInstallDrv();
		HANDLE hDriver = NULL;
		GetFinalPathNameByHandleDef pGetFinalPathNameByHandle = 0;
		if (!hDriver)
		{
			pGetFinalPathNameByHandle = (GetFinalPathNameByHandleDef)GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "GetFinalPathNameByHandleW");
		}
		// Now walk all handles
		EnumerateOpenedFiles(csPath, CallBackProc, pUserContext, hDriver, pGetFinalPathNameByHandle);
		if (hDriver)
		{	// Time to wind up		
			// StopAndUninstallDrv( hDriver );
		}
	}
}

UINT g_CurrentIndex = 0;
struct THREAD_PARAMS
{
	PSYSTEM_HANDLE_INFORMATION pSysHandleInformation;
	GetFinalPathNameByHandleDef pGetFinalPathNameByHandle;
	LPTSTR lpPath;
	int nFileType;
	HANDLE hStartEvent;
	HANDLE hFinishedEvent;
	bool bStatus;
};

DWORD WINAPI ThreadProc(LPVOID lParam)
{
	THREAD_PARAMS* pThreadParam = (THREAD_PARAMS*)lParam;

	GetFinalPathNameByHandleDef pGetFinalPathNameByHandle = pThreadParam->pGetFinalPathNameByHandle;
	for (g_CurrentIndex; g_CurrentIndex < pThreadParam->pSysHandleInformation->dwCount;)
	{

		WaitForSingleObject(pThreadParam->hStartEvent, INFINITE);
		ResetEvent(pThreadParam->hStartEvent);
		pThreadParam->bStatus = false;
		SYSTEM_HANDLE& sh = pThreadParam->pSysHandleInformation->Handles[g_CurrentIndex];
		g_CurrentIndex++;
		HANDLE hDup = (HANDLE)sh.wValue;
		HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, sh.dwProcessId);
		if (hProcess)
		{
			BOOL b = DuplicateHandle(hProcess, (HANDLE)sh.wValue, GetCurrentProcess(), &hDup, 0, FALSE, DUPLICATE_SAME_ACCESS);
			if (!b)
			{
				hDup = (HANDLE)sh.wValue;
			}
			CloseHandle(hProcess);
		}
		DWORD dwRet = pGetFinalPathNameByHandle(hDup, pThreadParam->lpPath, MAX_PATH, 0);
		if (hDup && (hDup != (HANDLE)sh.wValue))
		{
			CloseHandle(hDup);
		}
		if (dwRet)
		{
			pThreadParam->bStatus = true;
		}
		SetEvent(pThreadParam->hFinishedEvent);

	}
	return 0;
}

void EnumerateOpenedFiles(wstring& csPath, OF_CALLBACK CallBackProc, UINT_PTR pUserContext, HANDLE hDriver,
	GetFinalPathNameByHandleDef pGetFinalPathNameByHandle)
{
	int nFileType = XP_FILETYPE;
	OSVERSIONINFO info = { 0 };
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&info);
	if (info.dwMajorVersion == 6 &&
		info.dwMinorVersion == 0)
	{
		nFileType = VISTA_FILETYPE;
	}
	LPCTSTR lpPath = csPath.c_str();

	TCHAR csShortName[MAX_PATH] = { 0 };
	GetShortPathName(csPath.c_str(), csShortName, MAX_PATH);
	_tcslwr_s(csShortName);

	bool bShortPath = false;
	LPCTSTR lpShortPath = csShortName;
	if (csPath != csShortName && csShortName[0]!=0)
	{
		bShortPath = true;
	}

	HMODULE hModule = GetModuleHandle(_T("ntdll.dll"));
	PNtQuerySystemInformation NtQuerySystemInformation = (PNtQuerySystemInformation)GetProcAddress(hModule, "NtQuerySystemInformation");
	if (0 == NtQuerySystemInformation)
	{
		OutputDebugString(L"Getting proc of NtQuerySystemInformation failed");
		return;
	}

	// Get the list of all handles in the system
	PSYSTEM_HANDLE_INFORMATION pSysHandleInformation = new SYSTEM_HANDLE_INFORMATION;
	DWORD size = sizeof(SYSTEM_HANDLE_INFORMATION);
	DWORD needed = 0;
	NTSTATUS status = NtQuerySystemInformation(SystemHandleInformation, pSysHandleInformation, size, &needed);
	if (!NT_SUCCESS(status))
	{
		if (0 == needed)
		{
			return;// some other error
		}
		// The previously supplied buffer wasn't enough.
		delete pSysHandleInformation;
		size = needed + 1024;
		pSysHandleInformation = (PSYSTEM_HANDLE_INFORMATION)new BYTE[size];
		status = NtQuerySystemInformation(SystemHandleInformation, pSysHandleInformation, size, &needed);
		if (!NT_SUCCESS(status))
		{
			// some other error so quit.
			delete pSysHandleInformation;
			return;
		}
	}

	if (pGetFinalPathNameByHandle)// there is no driver, we have do it ugly way
	{
		g_CurrentIndex = 0;
		TCHAR tcFileName[MAX_PATH + 1];
		THREAD_PARAMS ThreadParams;
		ThreadParams.lpPath = tcFileName;
		ThreadParams.nFileType = nFileType;
		ThreadParams.pGetFinalPathNameByHandle = pGetFinalPathNameByHandle;
		ThreadParams.pSysHandleInformation = pSysHandleInformation;
		ThreadParams.hStartEvent = ::CreateEvent(0, TRUE, FALSE, 0);
		ThreadParams.hFinishedEvent = ::CreateEvent(0, TRUE, FALSE, 0);
		HANDLE ThreadHandle = 0;
		while (g_CurrentIndex < pSysHandleInformation->dwCount)
		{
			if (!ThreadHandle)
			{
				ThreadHandle = CreateThread(0, 0, ThreadProc, &ThreadParams, 0, 0);
			}
			ResetEvent(ThreadParams.hFinishedEvent);
			SetEvent(ThreadParams.hStartEvent);
			if (WAIT_TIMEOUT == WaitForSingleObject(ThreadParams.hFinishedEvent, 100))
			{
				wstring csError;
				std::wostringstream wos;
				// csError.Format(L"Query hang for handle %d", (int)pSysHandleInformation->Handles[g_CurrentIndex - 1].wValue);
				wos << L"Query hang for handle " << (int)pSysHandleInformation->Handles[g_CurrentIndex - 1].wValue;
				csError = wos.str();

				OutputDebugString(csError.c_str());
				TerminateThread(ThreadHandle, 0);
				CloseHandle(ThreadHandle);
				ThreadHandle = 0;
				continue;
			}
			if (!ThreadParams.bStatus)
			{
				continue;
			}
			int nCmpStart = 4;
			wstring csFileName(&ThreadParams.lpPath[nCmpStart]);
			MakeLower(csFileName);
			if (0 != _tcsncmp(lpPath, csFileName.c_str(), csPath.length()))
			{
				continue;
			}
			OF_INFO_t stOFInfo;
			stOFInfo.dwPID = pSysHandleInformation->Handles[g_CurrentIndex - 1].dwProcessId;
			stOFInfo.lpFile = csFileName.c_str();
			stOFInfo.hFile = (HANDLE)pSysHandleInformation->Handles[g_CurrentIndex - 1].wValue;
			CallBackProc(&stOFInfo, pUserContext);
		}
		if (ThreadHandle)
		{
			if (WAIT_TIMEOUT == WaitForSingleObject(ThreadHandle, 1000))
			{
				TerminateThread(ThreadHandle, 0);
			}
			CloseHandle(ThreadHandle);
		}
		CloseHandle(ThreadParams.hStartEvent);
		CloseHandle(ThreadParams.hFinishedEvent);
		return;
	}

	// Walk through the handle list
	for (DWORD i = 0; i < pSysHandleInformation->dwCount; i++)
	{
		SYSTEM_HANDLE& sh = pSysHandleInformation->Handles[i];
		if (sh.bObjectType != nFileType)// Under windows XP file handle is of type 28
		{
			continue;
		}

		wstring csFileName;
		wstring csDir;
		if (hDriver)
		{
			HANDLE_INFO stHandle = { 0 };
			ADDRESS_INFO stAddress;
			stAddress.pAddress = sh.pAddress;
			DWORD dwReturn = 0;
			BOOL bSuccess = DeviceIoControl(
				hDriver, 
				IOCTL_LISTDRV_BUFFERED_IO,
				&stAddress, 
				sizeof(ADDRESS_INFO),
				&stHandle,
				sizeof(HANDLE_INFO), 
				&dwReturn, 
				NULL);


			if (bSuccess && stHandle.tcFileName[0] != 0 &&
				stHandle.uType != FILE_DEVICE_SOUND &&
				stHandle.uType != FILE_DEVICE_NAMED_PIPE)
			{

				if (stHandle.uType != FILE_DEVICE_NETWORK_FILE_SYSTEM)
				{
					// Get the drive name from the dos device name
					if (!GetDrive((LPCTSTR)stHandle.tcDeviceName, csFileName, true))
					{
						OutputDebugString(L"GetDrive failed");
					}
					csFileName += (LPCTSTR)stHandle.tcFileName;
				}
				else
				{
					csFileName = _T("\\");
					csFileName += (LPCTSTR)stHandle.tcFileName;
				}
			}
			else
			{
				continue;
			}
		}
		else
		{
			return;
		}


		MakeLower(csFileName);
		// Check whether the file belongs to the specified folder
		// 		if( -1 == csFileName.Find( csPath ))
		// 		{
		// 			if( bShortPath )
		// 			{
		// 				// Some times the file name may be in short path form.
		// 				if( -1 == csFileName.Find( csShortName ))
		// 				{
		// 					continue;
		// 				}
		// 			}
		// 			else
		// 			{
		// 				continue;
		// 			}
		// 		}

		if (0 != _tcsncmp(lpPath, csFileName.c_str(), csPath.length()))
		{
			if (bShortPath)
			{
				// Some times the file name may be in short path form.
				if (0 != _tcsncmp(lpShortPath, csFileName.c_str(), lstrlen(csShortName)))
				{
					continue;
				}
			}
			else
			{
				continue;
			}
		}
		OF_INFO_t stOFInfo;
		stOFInfo.dwPID = sh.dwProcessId;
		stOFInfo.lpFile = csFileName.c_str();
		stOFInfo.hFile = (HANDLE)sh.wValue;
		CallBackProc(&stOFInfo, pUserContext);
	}
	delete pSysHandleInformation;

}


void EnumerateLoadedModules(wstring& csPath, OF_CALLBACK CallBackProc, UINT_PTR pUserContext)
{
	TCHAR csShortName[MAX_PATH]; csShortName[0] = 0;
	GetShortPathName(csPath.c_str(), csShortName, MAX_PATH);
	_tcslwr_s(csShortName);

	bool bShortPath = false;
	if (csShortName != csPath && csShortName[0])
	{
		bShortPath = true;
	}

	DWORD dwsize = 300;
	PDWORD pDwId = (PDWORD)new BYTE[dwsize];
	DWORD dwRetruned = dwsize;
	// Enum all the process first
	while (1)
	{
		EnumProcesses(pDwId, dwsize, &dwRetruned);
		if (dwsize > dwRetruned)
		{
			break;
		}
		delete pDwId;
		dwsize += 50;
		pDwId = (PDWORD)new BYTE[dwsize];
	}
	int nCount = dwRetruned / sizeof(DWORD);
	int nItemCount = -1;
	// Enumerate modules of the above process
	for (int nIdx = 0; nIdx < nCount; nIdx++)
	{
		if (0 != pDwId[nIdx])
		{
			HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
			MODULEENTRY32 me32;
			// Take a snapshot of all modules in the specified process. 
			hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pDwId[nIdx]);
			if (hModuleSnap == INVALID_HANDLE_VALUE)
			{
				continue;
			}
			me32.dwSize = sizeof(MODULEENTRY32);
			if (!Module32First(hModuleSnap, &me32))
			{
				CloseHandle(hModuleSnap);
				continue;
			}
			bool bFirst = true;
			PROCESS_INFO_t stInfo;
			do
			{
				wstring csModule;
				if (bFirst)
				{
					// First module is always the exe name
					bFirst = false;
					if (!PathFileExists(me32.szExePath))
					{
						TCHAR tcFileName[MAX_PATH];
						HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, TRUE, pDwId[nIdx]);
						if (GetProcessImageFileName(hProcess, tcFileName, MAX_PATH))
						{
							GetDrive(tcFileName, csModule, false);
						}
						CloseHandle(hProcess);
					}
					else
					{
						csModule = me32.szExePath;
					}
					MakeLower(csModule);
				}
				else
				{
					csModule = me32.szExePath;
					MakeLower(csModule);
				}
				//if (-1 == csModule.Find(csPath))
				if (csModule.find(csPath) == std::wstring::npos)
				{
					if (bShortPath)
					{
						if (csModule.find(csShortName) == std::wstring::npos)
						{
							continue;
						}
					}
					else
					{
						continue;
					}
				}
				OF_INFO_t stOFInfo;
				stOFInfo.dwPID = pDwId[nIdx];
				stOFInfo.lpFile = csModule.c_str();
				CallBackProc(&stOFInfo, pUserContext);
			} while (Module32Next(hModuleSnap, &me32));
			CloseHandle(hModuleSnap);
		}
	}
	delete pDwId;
}