#include "stdafx.h"

#include <windows.h>
#include <wininet.h>
#include <urlhist.h>
#include <shlguid.h>

BOOL DeleteIECache(BOOL bNormal, BOOL bCookie, BOOL bHistory) 
{
    BOOL bResult = FALSE;
    BOOL bDone = FALSE;
    LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;  
 
    DWORD  dwTrySize, dwEntrySize = 4096; // start buffer size    
    HANDLE hCacheDir = NULL;    
    DWORD  dwError = ERROR_INSUFFICIENT_BUFFER;
    
	if( bNormal || bCookie || bHistory)
	{
		do 
		{                               
			switch (dwError)
			{
				// need a bigger buffer
				case ERROR_INSUFFICIENT_BUFFER: 
					delete [] lpCacheEntry;
					lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
					lpCacheEntry->dwStructSize = dwEntrySize;
					dwTrySize = dwEntrySize;
					BOOL bSuccess;
					if (hCacheDir == NULL)                
                  
						bSuccess = (hCacheDir 
						  = FindFirstUrlCacheEntry(NULL, lpCacheEntry,
						  &dwTrySize)) != NULL;
					else
						bSuccess = FindNextUrlCacheEntry(hCacheDir, lpCacheEntry, &dwTrySize);

					if (bSuccess)
						dwError = ERROR_SUCCESS;    
					else
					{
						dwError = GetLastError();
						dwEntrySize = dwTrySize; // use new size returned
					}
					break;

				 // we are done
				case ERROR_NO_MORE_ITEMS:
					bDone = TRUE;
					bResult = TRUE;                
					break;

				 // we have got an entry
				case ERROR_SUCCESS:                       

					//Normal
					if( bNormal && bCookie )
						 DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
					else if( bNormal && !bCookie )
					{

						if( !(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY) )
						{//not cookie
							DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
						}
					}
					else if( !bNormal && bCookie )
					{
						if( (lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY) )
						{//cookie
							DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
						}
					}

					if( bHistory )
					{
						if( lpCacheEntry->CacheEntryType & URLHISTORY_CACHE_ENTRY )
						{// History
							DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
						}
					}

					dwTrySize = dwEntrySize;
					if (FindNextUrlCacheEntry(hCacheDir, lpCacheEntry, &dwTrySize))
						dwError = ERROR_SUCCESS;          
     
					else
					{
						dwError = GetLastError();
						dwEntrySize = dwTrySize; // use new size returned
					}                    
					break;

				// unknown error
				default:
					bDone = TRUE;                
					break;
			}

			if (bDone)
			{   
				delete [] lpCacheEntry; 
				if (hCacheDir)
					FindCloseUrlCache(hCacheDir);         
                                  
			}
		} while (!bDone);
	}
	else
	{
		bResult = TRUE;
	}

	if( bHistory )
	{
		OleInitialize(NULL);
		IUrlHistoryStg2* pUrlHistoryStg2 = NULL;
		HRESULT hr = CoCreateInstance(CLSID_CUrlHistory,
			NULL, CLSCTX_INPROC, IID_IUrlHistoryStg2,
			(void**)&pUrlHistoryStg2);
		if (SUCCEEDED(hr))
		{         
		   hr = pUrlHistoryStg2->ClearHistory(); 
		   pUrlHistoryStg2->Release();
		}
		
		bResult &= SUCCEEDED(hr);
		OleUninitialize();
	}

    return bResult;
}


