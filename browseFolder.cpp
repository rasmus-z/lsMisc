#include "stdafx.h"

#include <windows.h>
#include <comdef.h> 
#include <tchar.h>
#include <objbase.h>
#include <shlobj.h>
#include <ShObjIdl.h>

#include <io.h>

#include "browseFolder.h"

//#define TRACE(t1,t2) (void)0

#pragma comment(lib,"Shell32.lib")


_COM_SMARTPTR_TYPEDEF(IFileOpenDialog, __uuidof(IFileOpenDialog));
_COM_SMARTPTR_TYPEDEF(IShellItem, __uuidof(IShellItem));
_COM_SMARTPTR_TYPEDEF(IFileDialog, __uuidof(IFileDialog));

static IShellItemPtr getShellItemFromPath(LPWSTR pPath)
{
	typedef HRESULT (*fnSHCreateItemFromParsingName)(
		PCWSTR   pszPath,
		IBindCtx *pbc,
		REFIID   riid,
		void     **ppv
		);
	IShellItemPtr pRet;
	HMODULE h = LoadLibrary(L"shell32.dll");
	if(h)
	{
		fnSHCreateItemFromParsingName fn = (fnSHCreateItemFromParsingName)
			GetProcAddress(h, "SHCreateItemFromParsingName");
		if(fn)
		{
			fn(pPath, NULL, IID_IShellItem, (void**)&pRet);
		}
		FreeLibrary(h);
	}

	return pRet;
}
static bool bfVista(HWND hWnd, LPCTSTR lpszTitle, LPTSTR pFolder, bool& handled)
{
	handled = false;
	if (FAILED(CoInitializeEx(NULL,
		COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
	{
		return false;
	}
	struct CoUn {
		~CoUn() {
			CoUninitialize();
		}
	} coUn;

	IFileDialogPtr spFileDialog;
	HRESULT hr = spFileDialog.CreateInstance(__uuidof(FileOpenDialog));
	if (FAILED(hr))
		return false;

	DWORD dwOptions;
	if (FAILED(spFileDialog->GetOptions(&dwOptions)))
		return false;


	if (FAILED(spFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS)))
		return false;

	if (FAILED(spFileDialog->SetTitle(lpszTitle)))
		return false;


	IShellItemPtr pDefault;
	if(pFolder[0])
	{
		DWORD dwAttr = GetFileAttributes(pFolder);
		if(dwAttr != 0xFFFFFFFF && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
		{
			pDefault = getShellItemFromPath(pFolder);
		}
	}


	if(pDefault != NULL)
	{
		spFileDialog->SetDefaultFolder(pDefault);
	}

	bool ret;
	hr = spFileDialog->Show(hWnd);
	if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
	{
		handled = true;
		return false;
	}
	else if (SUCCEEDED(hr)) 
	{
		ret = true;
	}
	else
	{
		return false;
	}




	IShellItemPtr psi;
	if (FAILED(spFileDialog->GetResult(&psi)))
		return false;

	LPWSTR szFileName = NULL;
	if (FAILED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &szFileName)))
		return false;

	lstrcpy(pFolder, szFileName);
	CoTaskMemFree(szFileName);

	handled = true;
	return ret;
}


struct FOLDER_PROPS
{
	LPCTSTR lpszTitle;
	LPCTSTR lpszInitialFolder;
	UINT ulFlags;
};

static int CALLBACK BrowseCallbackProc(HWND hWnd,		// Window handle to the browse dialog box
									   UINT uMsg,		// Value identifying the event
									   LPARAM lParam,	// Value dependent upon the message 
									   LPARAM lpData)	// Application-defined value that was 
									   // specified in the lParam member of the 
									   // BROWSEINFO structure
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:		// sent when the browse dialog box has finished initializing. 
		{
			// TRACE(_T("hWnd=%X\n"), hWnd);

			// remove context help button from dialog caption
			LONG lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
			lStyle &= ~DS_CONTEXTHELP;
			::SetWindowLong(hWnd, GWL_STYLE, lStyle);
			lStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
			lStyle &= ~WS_EX_CONTEXTHELP;
			::SetWindowLong(hWnd, GWL_EXSTYLE, lStyle);

			FOLDER_PROPS *fp = (FOLDER_PROPS *)lpData;
			if (fp)
			{
				if (fp->lpszInitialFolder && (fp->lpszInitialFolder[0] != _T('\0')))
				{
					// set initial directory
					::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)fp->lpszInitialFolder);
				}

				if (fp->lpszTitle && (fp->lpszTitle[0] != _T('\0')))
				{
					// set window caption
					::SetWindowText(hWnd, fp->lpszTitle);
				}
			}

			// SizeBrowseDialog(hWnd, fp);
		}
		break;

	case BFFM_SELCHANGED:		// sent when the selection has changed
		{
			TCHAR szDir[MAX_PATH * 2] = { 0 };

			// fail if non-filesystem
			BOOL bRet = SHGetPathFromIDList((LPITEMIDLIST)lParam, szDir);
			if (bRet)
			{
				// fail if folder not accessible
				if (_taccess(szDir, 00) != 0)
				{
					bRet = FALSE;
				}
				else
				{
					SHFILEINFO sfi;
					::SHGetFileInfo((LPCTSTR)lParam, 0, &sfi, sizeof(sfi),
						SHGFI_PIDL | SHGFI_ATTRIBUTES);
					// TRACE(_T("dwAttributes=0x%08X\n"), sfi.dwAttributes);

					// fail if pidl is a link
					if (sfi.dwAttributes & SFGAO_LINK)
					{
						// TRACE(_T("SFGAO_LINK\n"),0);
						bRet = FALSE;
					}
				}
			}

			// if invalid selection, disable the OK button
			if (!bRet)
			{
				::EnableWindow(GetDlgItem(hWnd, IDOK), FALSE);
			}

			// TRACE(_T("szDir=%s\n"), szDir);
		}
		break;
	}

	return 0;
}

BOOL bfXp(HWND hParent, LPCTSTR lpszTitle, LPTSTR pFolder)
{
	BOOL bRet = FALSE;

	TCHAR szDisplayName[_MAX_PATH];
	TCHAR szBuffer[_MAX_PATH];

	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = hParent;
	// set root at Desktop
	browseInfo.pidlRoot = NULL;
	browseInfo.pszDisplayName = szDisplayName;
	browseInfo.lpszTitle = lpszTitle;
	browseInfo.ulFlags = BIF_RETURNONLYFSDIRS;   // also passed in

	FOLDER_PROPS fprs = { 0 };
	fprs.lpszInitialFolder = pFolder;
	fprs.lpszTitle = lpszTitle;
	browseInfo.lpfn = BrowseCallbackProc;
	browseInfo.lParam = (LPARAM)&fprs;

	LPITEMIDLIST lpItemIDList;
	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
	{
		// Get the path of the selected folder from the
		// item ID list.
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer) && szBuffer[0] != '\0')
		{
			lstrcpy(pFolder, szBuffer);
			bRet = TRUE;
		}

		// lpMalloc->Free(lpItemIDList);
		CoTaskMemFree(lpItemIDList);
	}

	return bRet;
}


BOOL browseFolder(HWND hParent, LPCTSTR lpszTitle, LPTSTR pFolder)
{
	bool handled = false;
	bool ret = bfVista(hParent, lpszTitle, pFolder, handled);
	if (handled)
		return ret;
	return bfXp(hParent, lpszTitle, pFolder);
}