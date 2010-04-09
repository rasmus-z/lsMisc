#pragma warning(disable:4786)
#include <windows.h>
#include <map>
#include <comdef.h>
#include "getFromHtmlLikeDataW.h"
using namespace std;
void getFromHtmlLikeDataW(const WCHAR* pDataO, map<_bstr_t, _bstr_t>& headerMap,
						  WCHAR** ppHtml)
{
	if (ppHtml) 
		*ppHtml=0;
	if ( pDataO==NULL || pDataO[0]==0 )
		return;
	
//	size_t checkcount = 0;

	WCHAR* pDataOrig = wcsdup(pDataO);
	WCHAR* pData = pDataOrig;
	WCHAR* pOUT = pDataOrig;

	if ( ( *((BYTE*)pData)==0xff && *((BYTE*)pData+1)==0xfe) )
	{ // little
		pData++;
		pOUT++;
	}
	else 	if ( ( *((BYTE*)pData)==0xfe && *((BYTE*)pData+1)==0xff) )
	{
		pData++;
		pOUT++;
		BYTE* pByte = (BYTE*)pData;
		while( !(*pByte==0 && *(pByte+1)==0) )
		{
			BYTE tmp = *pByte;
			*pByte = *(pByte+1);
			*(pByte+1) = tmp;
			pByte+=2;
		}
	}

	WCHAR* p = wcstok(pData, L"\r\n");
	while( p )
	{
		WCHAR* pT = wcsstr(p, L": ");
		pOUT = p;
		if ( pT==NULL )
		{
			
			break;
		}

		*pT=0;


		_bstr_t key = p;

		pT++;
		pT++;
		WCHAR* pTT = pT;
		for( ; *pTT ; ++pTT)
		{
			if(*pTT==L'\r' || *pTT==L'\n')
			{
				*pTT=0;
				break;
			}
		}
		_bstr_t val = pT;
		headerMap.insert(make_pair(key,val));

		p = wcstok(NULL, L"\r\n");
	}
	pOUT = p;
	if ( pOUT )
		pOUT = pOUT-pDataOrig+(WCHAR*)pDataO;
	free(pDataOrig);
	pData =NULL;

	if ( ppHtml )
	{
		*ppHtml = pOUT;
		/**
		do {

			list<LPWSTR> intlist;
			LPWSTR p1,p2,p3,p4;
			p1=wcsstr(pDataO + checkcount, L"\r\n\r");
			p2=wcsstr(pDataO + checkcount, L"\r\n\n");
			p3=wcsstr(pDataO + checkcount, L"\r\r");
			p4=wcsstr(pDataO + checkcount, L"\n\n");
			intlist.push_back(p1);
			intlist.push_back(p2);
			intlist.push_back(p3);
			intlist.push_back(p4);
			intlist.sort();
			
			list<LPWSTR>::iterator it = intlist.begin();
			for(; it != intlist.end() ; ++it)
			{
				if (*it==NULL)
					continue;
				else
				{
					*pN = *it-pDataO;
					break;
				}
			}
			if (*pN==0)
				*pN = checkcount;
		} while(0);
		**/

//		*pN = checkcount;
	}
	return;
}