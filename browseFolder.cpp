#include <windows.h>
#include <tchar.h>
#include <objbase.h>
#include <shlobj.h>

#include <io.h>

#include "browseFolder.h"

#define TRACE(t1,t2) (void)0


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
			TRACE(_T("hWnd=%X\n"), hWnd);

			// remove context help button from dialog caption
			LONG lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
			lStyle &= ~DS_CONTEXTHELP;
			::SetWindowLong(hWnd, GWL_STYLE, lStyle);
			lStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
			lStyle &= ~WS_EX_CONTEXTHELP;
			::SetWindowLong(hWnd, GWL_EXSTYLE, lStyle);

			FOLDER_PROPS *fp = (FOLDER_PROPS *) lpData;
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
			TCHAR szDir[MAX_PATH*2] = { 0 };

			// fail if non-filesystem
			BOOL bRet = SHGetPathFromIDList((LPITEMIDLIST) lParam, szDir);
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
					TRACE(_T("dwAttributes=0x%08X\n"), sfi.dwAttributes);

					// fail if pidl is a link
					if (sfi.dwAttributes & SFGAO_LINK)
					{
						TRACE(_T("SFGAO_LINK\n"));
						bRet = FALSE;
					}
				}
			}

			// if invalid selection, disable the OK button
			if (!bRet)
			{
				::EnableWindow(GetDlgItem(hWnd, IDOK), FALSE);
			}

			TRACE(_T("szDir=%s\n"), szDir);
		}
		break;
	}
         
	return 0;
}

BOOL browseFolder( HWND hParent, LPCTSTR lpszTitle, LPTSTR pFolder )
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
	browseInfo.ulFlags = BIF_RETURNONLYFSDIRS ;   // also passed in
	
	FOLDER_PROPS fprs = {0};
	fprs.lpszInitialFolder = pFolder;
	fprs.lpszTitle = lpszTitle;
	browseInfo.lpfn = BrowseCallbackProc;
	browseInfo.lParam = (LPARAM)&fprs;

	LPITEMIDLIST lpItemIDList;
	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
	{
		// Get the path of the selected folder from the
		// item ID list.
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer) && szBuffer[0] != '\0' )
		{
			lstrcpy(pFolder, szBuffer);
			bRet = TRUE;
		}

		// lpMalloc->Free(lpItemIDList);
		CoTaskMemFree(lpItemIDList);
	}

	return bRet;
}


