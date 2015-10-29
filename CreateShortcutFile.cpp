#include <windows.h>
#include <tchar.h>
#include <Shobjidl.h>
#include <Objbase.h>
#include <ShlGuid.h>

BOOL CreateShortcutFile(LPCTSTR pszShortcutFile, 
							   LPCTSTR pszTargetFile,
							   LPCTSTR pszCurDir,
							   LPCTSTR pszArg,
						       int iIconLocation)
{
	BOOL bFailed = TRUE;
	HRESULT hr;
	IShellLink* pShellLink = NULL;
	CoInitialize(NULL);
    hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&pShellLink);
	if(SUCCEEDED(hr) && pShellLink)
	{
		IPersistFile* pPFile = NULL;
		hr = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID*)&pPFile);
		if(SUCCEEDED(hr) && pPFile)
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
				bFailed |= FAILED(pPFile->Save(pszShortcutFile, TRUE));
			}
			pPFile->Release();
		}
		pShellLink->Release();
	}
	CoUninitialize();
	return !bFailed;
}

