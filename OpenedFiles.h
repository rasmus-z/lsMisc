//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#pragma once

#include <Windows.h>

#ifndef _VECTOR_
#pragma message(" Please include vector")
#endif

enum OF_TYPE
{
	FILES_ONLY = 1,
	MODULES_ONLY = 2,
	ALL_TYPES = 3
};

struct OF_INFO_t
{
	DWORD dwPID;
	LPCWSTR lpFile;
	HANDLE hFile;
};

struct OPENEDFILEINFO
{
	DWORD dwPID;
	TCHAR filename[MAX_PATH];
	HANDLE hFile;
};

typedef void (CALLBACK* OF_CALLBACK)(OF_INFO_t* pOpenedFileInf0, UINT_PTR uUserContext);


void ShowOpenedFiles(LPCWSTR lpPath);
void GetOpenedFiles(LPCWSTR lpPath,
	OF_TYPE Filter,
	OF_CALLBACK CallBackProc,
	UINT_PTR pUserContext);

std::wstring GetPathFromProcessID(const DWORD dwID);
void GetOpenedFilesSimple(LPCTSTR pFilter, std::vector<OPENEDFILEINFO>& v);