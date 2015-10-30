#include <windows.h>
#include <tchar.h>
#include <Shobjidl.h>
#include <Objbase.h>
#include <ShlGuid.h>
#include <comdef.h>
#include <shlwapi.h>

static BOOL CheckShortcutFile(LPCTSTR pszShortcutFile, 
							   LPCTSTR pszTargetFile,
							   LPCTSTR pszCurDir,
							   LPCTSTR pszArg,
						       int iIconLocation)
{
	BOOL bFailed = TRUE;
	HRESULT hr;
	IShellLinkWPtr pShellLink = NULL;
	CoInitialize(NULL);
    hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&pShellLink);
	int bufflen = max(lstrlen(pszShortcutFile), lstrlen(pszTargetFile));
	bufflen = max(bufflen, lstrlen(pszCurDir));
	bufflen = max(bufflen, lstrlen(pszArg));
	bufflen++;
	LPTSTR pBuffer = (LPTSTR)malloc(bufflen * sizeof(TCHAR));
	if(SUCCEEDED(hr) && pShellLink != NULL)
	{
		IPersistFilePtr pPFile = NULL;
		hr = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID*)&pPFile);
		if(SUCCEEDED(hr) && pPFile != NULL)
		{
			hr=pPFile->Load(pszShortcutFile, 0);
			if(SUCCEEDED(hr))
			{
				bFailed = FALSE;
				WIN32_FIND_DATA wfd;
				if(FAILED(pShellLink->GetPath(pBuffer,bufflen,&wfd,0)))
					return FALSE;
				

				int iI=0;
				if(FAILED(pShellLink->GetIconLocation( pBuffer, bufflen, &iI )))
					return FALSE;
				if(lstrcmp(pBuffer, pszTargetFile))
					return FALSE;
				if( iI != iIconLocation )
					return FALSE;


				if( FAILED(pShellLink->GetArguments(pBuffer, bufflen)) )
					return FALSE;
				if(lstrcmp(pBuffer,pszArg)!=0)
					return FALSE;

				
				if(FAILED(pShellLink->GetWorkingDirectory(pBuffer,bufflen)))
					return FALSE;
				if( lstrcmp(pBuffer,pszCurDir) !=0)
					return FALSE;
			}
		}
	}
	free(pBuffer);
	CoUninitialize();
	return !bFailed;
}

BOOL CreateShortcutFile(LPCTSTR pszShortcutFile, 
							   LPCTSTR pszTargetFile,
							   LPCTSTR pszCurDir,
							   LPCTSTR pszArg,
						       int iIconLocation)
{
	BOOL bFailed = TRUE;
	HRESULT hr;

	TCHAR szTempDir[MAX_PATH] = {0};
	DWORD dwTP = GetTempPath(MAX_PATH, szTempDir);
	if(dwTP==0 || !PathIsDirectory(szTempDir))
	{
		return FALSE;
	}

	TCHAR szTempFile[MAX_PATH] = {0};
	CoInitialize(NULL);
	{
		IShellLinkWPtr pShellLink = NULL;
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&pShellLink);
		if(SUCCEEDED(hr) && pShellLink != NULL)
		{
			IPersistFilePtr pPFile = NULL;
			hr = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID*)&pPFile);
			if(SUCCEEDED(hr) && pPFile != NULL)
			{
				hr = pShellLink->SetPath(pszTargetFile);
				if(SUCCEEDED(hr))
				{
					bFailed = FALSE;
					if(iIconLocation != -1)
					{
						bFailed |= FAILED(pShellLink->SetIconLocation(pszTargetFile, iIconLocation));
					}

					bFailed |= FAILED(pShellLink->SetArguments(pszArg));
					bFailed |= FAILED(pShellLink->SetWorkingDirectory(pszCurDir));

					
					if(0==GetTempFileName(szTempDir, L"cbs", 0, szTempFile))
						return FALSE;

					bFailed |= FAILED(pPFile->Save(szTempFile, TRUE));
				}
			}
		}
	}
	CoUninitialize();
	if( bFailed )
		return FALSE;

	if(!CheckShortcutFile(szTempFile, 
							  pszTargetFile,
							  pszCurDir,
							  pszArg,
						      iIconLocation))
	{
		DeleteFile(szTempFile);
		return FALSE;
	}

	if(!MoveFileEx(szTempFile, pszShortcutFile, MOVEFILE_REPLACE_EXISTING))
		return FALSE;

	return TRUE;
}

