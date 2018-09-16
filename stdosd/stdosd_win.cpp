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


#include "StdAfx.h"
#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <winioctl.h>
#include <regex>

#include "stdosd.h"

#if !defined(REPARSE_DATA_BUFFER_HEADER_SIZE)
typedef struct _REPARSE_DATA_BUFFER {
	ULONG  ReparseTag;
	USHORT ReparseDataLength;
	USHORT Reserved;
	union {
		struct {
			USHORT SubstituteNameOffset;
			USHORT SubstituteNameLength;
			USHORT PrintNameOffset;
			USHORT PrintNameLength;
			ULONG  Flags;
			WCHAR  PathBuffer[1];
		} SymbolicLinkReparseBuffer;
		struct {
			USHORT SubstituteNameOffset;
			USHORT SubstituteNameLength;
			USHORT PrintNameOffset;
			USHORT PrintNameLength;
			WCHAR  PathBuffer[1];
		} MountPointReparseBuffer;
		struct {
			UCHAR  DataBuffer[1];
		} GenericReparseBuffer;
	};
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;
#  define REPARSE_DATA_BUFFER_HEADER_SIZE  FIELD_OFFSET(REPARSE_DATA_BUFFER, GenericReparseBuffer)
#endif // !defined(REPARSE_DATA_BUFFER_HEADER_SIZE)


namespace Ambiesoft {
	namespace stdosd {
		using namespace std;

		namespace {
			std::wstring stdGetFullPathName(LPCWSTR pPath)
			{
				std::wstring ret;
				if (pPath == NULL || pPath[0] == 0)
					return ret;

				DWORD dwReqChar = GetFullPathNameW(pPath, 0, NULL, NULL);
				wchar_t* pBuffer = new wchar_t[dwReqChar];
				pBuffer[0] = 0;
				DWORD retChar = GetFullPathNameW(pPath, dwReqChar, pBuffer, NULL);
				if (retChar > dwReqChar)
				{
					delete[] pBuffer;
					return ret;
				}

				ret = pBuffer;
				delete[] pBuffer;

				return ret;
			}

			std::vector<std::wstring> splitdir(const std::wstring& input, const std::wstring& regex) {
				// passing -1 as the submatch index parameter performs splitting
				std::wregex re(regex);
				std::wsregex_token_iterator
					first{ input.begin(), input.end(), re, -1 },
					last;
				return { first, last };
			}

			wstring resolveSingleDir(const wstring& input)
			{
				wstring result;

				DWORD dwFA = GetFileAttributes(input.c_str());
				if (dwFA == 0xFFFFFFFF)
					return input;
				if ((dwFA & FILE_ATTRIBUTE_REPARSE_POINT) == 0)
					return input;

				HANDLE handle = CreateFile(input.c_str(),
					FILE_READ_EA,
					FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
					0,
					OPEN_EXISTING,
					FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
					0);
				if (handle != INVALID_HANDLE_VALUE) {
					DWORD bufsize = MAXIMUM_REPARSE_DATA_BUFFER_SIZE;
					REPARSE_DATA_BUFFER *rdb = (REPARSE_DATA_BUFFER*)malloc(bufsize);
					DWORD retsize = 0;
					if (::DeviceIoControl(handle, FSCTL_GET_REPARSE_POINT, 0, 0, rdb, bufsize, &retsize, 0)) {
						if (rdb->ReparseTag == IO_REPARSE_TAG_MOUNT_POINT) {
							int length = rdb->MountPointReparseBuffer.SubstituteNameLength / sizeof(wchar_t);
							int offset = rdb->MountPointReparseBuffer.SubstituteNameOffset / sizeof(wchar_t);
							const wchar_t* PathBuffer = &rdb->MountPointReparseBuffer.PathBuffer[offset];
							result.assign(PathBuffer, length);
						}
						else if (rdb->ReparseTag == IO_REPARSE_TAG_SYMLINK) {
							int length = rdb->SymbolicLinkReparseBuffer.SubstituteNameLength / sizeof(wchar_t);
							int offset = rdb->SymbolicLinkReparseBuffer.SubstituteNameOffset / sizeof(wchar_t);
							const wchar_t* PathBuffer = &rdb->SymbolicLinkReparseBuffer.PathBuffer[offset];
							result.assign(PathBuffer, length);
						}
						// cut-off "//?/" and "/??/"
						if (result.size() > 4 && result.at(0) == L'\\' && result.at(2) == L'?' && result.at(3) == L'\\')
							result = result.substr(4); // mid(4);
					}
					free(rdb);
					CloseHandle(handle);
				}
				return result;
			}
		}
		std::wstring stdGetFullPathName(const std::wstring& ws)
		{
			return stdGetFullPathName(ws.c_str());
		}
		std::wstring resolveLink(const std::wstring& instring)
		{
			if (instring.empty())
				return instring;

			wstring full = stdGetFullPathName(instring);
			if (full.empty())
				return instring;

			//wchar_t drive[_MAX_DRIVE];
			//wchar_t dir[_MAX_DIR];
			//wchar_t name[FILENAME_MAX];
			//wchar_t ext[_MAX_EXT];
			//if (0 != _wsplitpath_s(full.c_str(),
			//	drive,
			//	dir,
			//	name,
			//	ext))
			//{
			//	return instring;
			//}

			if (PathIsUNC(full.c_str()))
				return instring;

			vector<wstring> dirs = splitdir(full, L"\\\\");
			wstring resultdir = dirs[0];

			for (size_t i = 1; i < (dirs.size()); ++i)
			{
				resultdir += L"\\";
				resultdir += dirs[i];

				resultdir = resolveSingleDir(resultdir);
			}

			return resultdir;
		}
	}
}