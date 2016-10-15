#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
//#include <objidl.h>
#include <shlobj.H>
//#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "ResolveLink.h"

BOOL ResolveLink(LPCTSTR lpszFileIn, HWND hWnd, LPTSTR target)
{
	BOOL retval = FALSE;
	if( SUCCEEDED(CoInitialize(NULL)) )
	{
		IShellLink* psl = NULL;
		if( SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_SERVER, IID_IShellLink,
			(LPVOID*)&psl)) )
		{
			IPersistFile *ppf = NULL;
			if (SUCCEEDED(psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf)))
			{
#ifdef UNICODE
				WCHAR* pIn = _wcsdup(lpszFileIn);
#else
				int nAllocLeng = lstrlen(lpszFileIn)+sizeof(WCHAR);
				WCHAR* pIn = new WCHAR[nAllocLeng];
				if( MultiByteToWideChar(CP_ACP, 0, lpszFileIn, -1, pIn, nAllocLeng) == 0 )
				{
					delete[] pIn;
					return FALSE;
				}
#endif
				{
					if (SUCCEEDED(ppf->Load(pIn, STGM_READ)))
					{
						if (SUCCEEDED(psl->Resolve(hWnd, SLR_ANY_MATCH)))
						{
							if( SUCCEEDED(psl->GetPath(target, MAX_PATH, NULL, 0)) )
							{
								retval = TRUE;
							}
						}
					}
				}
				delete [] pIn;
				ppf->Release();
			}
			psl->Release();
		}
		CoUninitialize();
	}
	return retval;
}