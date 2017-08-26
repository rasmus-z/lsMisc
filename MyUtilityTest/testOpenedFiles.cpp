#include "stdafx.h"


#include "../OpenedFiles.h"

using namespace std;

struct myOF_INFO_t
{
	DWORD dwPID;
	TCHAR filename[MAX_PATH];
	HANDLE hFile;
};

struct CallbackData
{
private:
	LPTSTR pFilter_;
public:
	CallbackData() :pFilter_(NULL){}
	~CallbackData()
	{
		free(pFilter_);
	}
	vector<myOF_INFO_t> v_;

	LPCTSTR getFilter() const {
		return pFilter_;
	}
	void setFilter(LPCTSTR pFilter)
	{
		pFilter_ = _tcsdup(pFilter);
		_tcslwr_s(pFilter_, lstrlen(pFilter)+1);
	}
};

static wstring getPathFromProcessID(const DWORD dwID)
{
	HANDLE processHandle = NULL;
	TCHAR filename[MAX_PATH];

	wstring ret;

	processHandle = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE,
		dwID);
	if (processHandle != NULL) 
	{
		if (GetModuleFileNameEx(processHandle, NULL, filename, MAX_PATH) == 0) 
		{
			return ret;
		}
		else 
		{
			ret = filename;
		}
		CloseHandle(processHandle);
	}
	return ret;
}

void CALLBACK mycallback(OF_INFO_t* pOpenedFileInfo, UINT_PTR uUserContext)
{
	CallbackData* cd = (CallbackData*)uUserContext;

	TCHAR szT1[MAX_PATH];
	lstrcpy(szT1, pOpenedFileInfo->lpFile);
	_tcslwr_s(szT1);

	if (!_tcsstr(szT1, cd->getFilter()))
		return;

	myOF_INFO_t t;
	t.dwPID = pOpenedFileInfo->dwPID;
	lstrcpy(t.filename, pOpenedFileInfo->lpFile);
	t.hFile = pOpenedFileInfo->hFile;
	cd->v_.push_back(t);
}

void testOpenedFiles()
{
	CallbackData cd;
	cd.setFilter(L"C:\\Users\\Bokkurin\\Desktop\\vvvvvvvvvvvvvvvvvvvvvvvvv.txt");

	GetOpenedFiles(
		L"",
		ALL_TYPES,
		mycallback,
		(UINT_PTR)&cd);

	wstring message;
	for each(const myOF_INFO_t& t in cd.v_)
	{
		message.append(t.filename);
		message.append(L"\r\n");

		message.append(getPathFromProcessID(t.dwPID));
		message.append(L"\r\n");
	}

	MessageBox(NULL, message.c_str(), L"testOpenedFiles", MB_ICONINFORMATION);
}