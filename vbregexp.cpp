#include <windows.h>
#include <tchar.h>
#include "vbregexp.h"

#import "vbregexp.tlb" no_namespace named_guids

static IRegExpPtr pReg;
BOOL vbregMatch(LPCTSTR subject, LPCTSTR regex)
{
	if(pReg==NULL)
	{
		CoInitialize(NULL); 
		HRESULT hr = pReg.CreateInstance(CLSID_RegExp);
		if(FAILED(hr))
		{
			return FALSE;
		} 
	}

	_bstr_t str(subject);
	_bstr_t pattern(regex);

	pReg->put_Global(VARIANT_TRUE); 
	pReg->put_Pattern(pattern); 
	
	IDispatchPtr pDisp=pReg->Execute(str); 
	if(pDisp)
	{ 
		IMatchCollectionPtr pCol;
		if(SUCCEEDED(pDisp->QueryInterface(IID_IMatchCollection,(void**)&pCol)))
		{ 
			LONG count=0; 
			if(SUCCEEDED(pCol->get_Count(&count)) && count > 0)
				return TRUE;
		}
	}

	return FALSE;
}


LPCTSTR vbregReplace(LPCTSTR source,
					 LPCTSTR replace, 
					 LPCTSTR regex,
					 BOOL bGlobal,
					 BOOL bCaseInsensitive)
{
	if(pReg==NULL)
	{
		CoInitialize(NULL); 
		HRESULT hr = pReg.CreateInstance(CLSID_RegExp);
		if(FAILED(hr))
		{
			return FALSE;
		} 
	}

	_bstr_t bstrSource(source);
	_bstr_t bstrReplace(replace);
	_bstr_t bstrRegex(regex);

	pReg->put_Global(bGlobal?VARIANT_TRUE:VARIANT_FALSE); 
	pReg->put_IgnoreCase(bCaseInsensitive?VARIANT_TRUE:VARIANT_FALSE); 
	pReg->put_Pattern(bstrRegex); 

	_bstr_t bstrRet = pReg->Replace(bstrSource, bstrReplace);

	return _tcsdup(bstrRet);
}


//			IDispatchPtr pDisp2;
//			for(long i=0; SUCCEEDED(pCol->get_Item(i, &pDisp2)) ; ++i)
//			{
//				IMatchPtr pMatch;
//				if(SUCCEEDED(pDisp2->QueryInterface(IID_IMatch,(void **)&pMatch)))
//				{ 
//					BSTR bstr; 
//					pMatch->get_Value(&bstr); 
//					_bstr_t value(bstr,false);

//					char *buf=_com_util::ConvertBSTRToString(value.GetBSTR()); 
//					_tprintf("all = %s\n",buf); 
//					delete[] buf; 
//				} 
//			}
