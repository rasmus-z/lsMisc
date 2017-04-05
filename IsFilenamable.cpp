#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <string>
#include "IsFilenamable.h"

using std::wstring;

wstring GetFileInamableChars()
{
	return L"/\\:*?\"<>|";
}
#ifdef __cplusplus_cli
System::String^ GetFileInamableCharsCLR()
{
	return gcnew System::String(GetFileInamableChars().c_str());
}
#endif
wstring GetRelativePathInamableChars()
{
	return L":*?\"<>|";
}
wstring GetFullPathInamableChars()
{
	return L"*?\"<>|";
}
#ifdef __cplusplus_cli
System::String^ GetFullPathInamableCharsCLR()
{
	return gcnew System::String(GetFullPathInamableChars().c_str());
}
#endif

BOOL makeFileNamable(LPTSTR pch)
{
	TCHAR* pTemp = pch;
	while( *pch != '\0' )
	{
		TCHAR* pOrgch = pch;
		if( *pch==_T('\\')
		||  *pch==_T(':')
		||  *pch==_T('*')
		||  *pch==_T('?')
		||  *pch==_T('\"')
		||  *pch==_T('<')
		||  *pch==_T('>')
		||  *pch==_T('|') )
		{
			do
			{
				*pOrgch = *(pOrgch+1);
				++pOrgch;
			}
			while( *pOrgch );
		}
		else
		{
			pch = (TCHAR*)_tcsinc(pch);
		}
	}
	
	pch = pTemp;
	while( *pch != _T('\0') )
	{
		if( *pch == _T('/') )
			*pch = _T('-');

		pch = (TCHAR*)_tcsinc(pch);
	}

	return TRUE;
}

BOOL IsFileNamble(LPCTSTR p)
{
	if(!p || !*p)
		return FALSE;

	return wcspbrk(p, GetFileInamableChars().c_str())==NULL;
}
BOOL IsRelativePathNamble(LPCTSTR p)
{
	if(!p || !*p)
		return FALSE;

	return wcspbrk(p, GetRelativePathInamableChars().c_str())==NULL;
}
BOOL IsFullPathNamble(LPCTSTR p)
{
	if(!p || !*p)
		return FALSE;

	return wcspbrk(p, GetRelativePathInamableChars().c_str())==NULL;
}




