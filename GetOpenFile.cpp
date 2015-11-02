#include "stdafx.h"

#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <string>
using namespace std;

#include"GetOpenFile.h"

#ifndef OFN_DONTADDTORECENT
	#define OFN_DONTADDTORECENT 0x2000000 
#endif

BOOL GetOpenFile(
				 HWND hWnd, 
				 LPCTSTR pFilter, 
				 LPCTSTR pInitialDir,
				 LPCTSTR pTitle,
				 wstring* fullpath, wstring* path)
{
	OPENFILENAME ofn;

	ZeroMemory( &ofn , sizeof( ofn));
	ofn.lStructSize = sizeof ( ofn );
	ofn.hwndOwner = hWnd;
	ofn.Flags = 
		OFN_DONTADDTORECENT		| 
		OFN_ENABLESIZING		|
		OFN_FILEMUSTEXIST		|
		OFN_HIDEREADONLY		|
		0;

	OFNBUFFER buff;
	if(pFilter)
	{
		lstrcpy(buff.pFilter_, pFilter);
		LPTSTR p = buff.pFilter_;
		while( *p && (p=_tcschr(p, L'|')) )
			*p++ = 0;
		ofn.lpstrFilter = buff.pFilter_;
	}

	ofn.lpstrFile = buff.pFile_;
	ofn.nMaxFile = OFNBUFFERSIZE;

	ofn.lpstrFileTitle = buff.pFileTitle_;
	ofn.nMaxFileTitle = OFNBUFFERSIZE;

	ofn.lpstrInitialDir = pInitialDir;

	ofn.lpstrTitle = pTitle;
	if(!GetOpenFileName( &ofn ))
		return FALSE;

	if(path)
		*path = ofn.lpstrFileTitle;

	*fullpath = ofn.lpstrFile;
	return TRUE;
}


BOOL GetSaveFile(
				 HWND hWnd, 
				 LPCTSTR pFilter, 
				 LPCTSTR pInitialDir,
				 LPCTSTR pInitialFile,
				 LPCTSTR pDefExt,
				 LPCTSTR pTitle,
				 wstring* fullpath, wstring* path)
{
	OPENFILENAME ofn;

	ZeroMemory( &ofn , sizeof( ofn));
	ofn.lStructSize = sizeof ( ofn );
	ofn.hwndOwner = hWnd;
	ofn.Flags = 
		OFN_DONTADDTORECENT		| 
		OFN_ENABLESIZING		|
		OFN_OVERWRITEPROMPT		|
		OFN_HIDEREADONLY		|
		0;

	OFNBUFFER buff;
	if(pFilter)
	{
		lstrcpy(buff.pFilter_, pFilter);
		LPTSTR p = buff.pFilter_;
		while( *p && (p=_tcschr(p, L'|')) )
			*p++ = 0;
		ofn.lpstrFilter = buff.pFilter_;
	}

	if(pInitialFile)
	{
		lstrcpy(buff.pFile_, pInitialFile);
	}

	ofn.lpstrFile = buff.pFile_;
	ofn.nMaxFile = OFNBUFFERSIZE;

	ofn.lpstrFileTitle = buff.pFileTitle_;
	ofn.nMaxFileTitle = OFNBUFFERSIZE;

	ofn.lpstrInitialDir = pInitialDir;

	ofn.lpstrDefExt = pDefExt;

	ofn.lpstrTitle = pTitle;
	if(!GetSaveFileName( &ofn ))
		return FALSE;

	if(path)
		*path = ofn.lpstrFileTitle;

	*fullpath = ofn.lpstrFile;
	return TRUE;
}
