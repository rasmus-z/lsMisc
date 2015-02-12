#define WIN32_LEAN_AND_MEAN		// Windows
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <Objbase.h>
#include <string>
using namespace std;

#pragma comment(lib,"Comctl32.lib")

static HICON ghIcon;

static BOOL NotifyIconize(UINT uID, DWORD dwMessage, LPCWSTR pInfoTitle , LPCWSTR pInfo)
{
	NOTIFYICONDATAW nid;
	ZeroMemory(&nid,sizeof(nid));
	nid.cbSize = sizeof(NOTIFYICONDATAW);
	nid.hWnd = NULL;
	nid.uID = uID;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | 0x00000010;
	nid.dwInfoFlags      = 0x00000001;
	nid.uTimeout         = 300;
	nid.uCallbackMessage = 0; //WM_APP_TRAYMESSAGE;
	nid.hIcon = ghIcon; //LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MAIN));
	lstrcpyW(nid.szTip, L"dater");
	if(pInfoTitle)
		lstrcpyW( nid.szInfoTitle, pInfoTitle );
	if(pInfo)
		lstrcpyW( nid.szInfo, pInfo );
	
	BOOL ret= Shell_NotifyIconW( dwMessage,&nid);
	//DestroyIcon(sfi.hIcon);
	return ret;
}

	



BOOL showballoon(wstring title, wstring text, wstring iconexe, UINT uTrayID)
{
	CoInitialize(NULL);

	if(!iconexe.empty())
	{
		SHFILEINFOW sfi={0};
		SHGetFileInfoW(iconexe.c_str(),
					   0,
					   &sfi, 
					   sizeof(SHFILEINFO), 
					   // SHGFI_SYSICONINDEX|
					   SHGFI_ICON|
					   SHGFI_SMALLICON);


		ghIcon = sfi.hIcon;
	} 	
	

	InitCommonControls();
	HWND hBalloon = CreateWindowW(L"tooltips_class32",
		NULL,
		WS_POPUP|0x02|0x40|0x01,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		NULL,
		NULL);
	if(!hBalloon)
	{
		MessageBoxW(NULL, L"balloon",NULL,MB_ICONERROR);
		return __LINE__;
	}
	if(!SendMessageW(hBalloon,
		TTM_SETTITLE, // Adds a standard icon and title string to a ToolTip    
		1,
		(LPARAM)L"title"))
	{
		MessageBoxW(NULL, L"balloon sendmessage",NULL,MB_ICONERROR);
		return __LINE__;
	}

	NotifyIconize(uTrayID,NIM_DELETE, NULL, NULL);
	if(!NotifyIconize(uTrayID,NIM_ADD, NULL, NULL))
	{
		MessageBoxA(NULL, "NotifyAdd",NULL,MB_ICONERROR);
		return __LINE__;
	}



	if(!NotifyIconize(uTrayID, NIM_MODIFY, title.c_str(), text.c_str() ))
	{
		MessageBoxA(NULL, "NotifyModify",NULL,MB_ICONERROR);
		return __LINE__;
	}
	Sleep(5000);
	NotifyIconize(uTrayID, NIM_DELETE, NULL, NULL);
	DestroyWindow(hBalloon);
	DestroyIcon(ghIcon);
	return 0;
}

