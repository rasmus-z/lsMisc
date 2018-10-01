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
#include <string>
#include <vector>
namespace Ambiesoft { namespace stdosd {
bool IsPathRoot(const char16_t* pPath);
bool IsBothEmptyOrNULL(const char16_t* p1, const char16_t* p2);
bool IsSamePathDepth(const char16_t* p1, const char16_t* p2);
bool IsPathChildIncluded(const char16_t* p1, const char16_t* p2, std::u16string* pDupPath = NULL);

bool checkDupPaths(const std::vector<std::u16string>& saPaths,
    std::u16string& left,
    std::u16string& right,
    std::u16string& common);

#if defined(_MSC_VER) || defined(__MINGW32__)
std::wstring stdPathCanonicalize(const wchar_t* pPath);
bool IsPathRoot(const wchar_t* pPath);
bool IsBothEmptyOrNULL(const wchar_t* p1, const wchar_t* p2);
bool IsSamePathDepth(const wchar_t* p1, const wchar_t* p2);
bool IsPathChildIncluded(const wchar_t* p1, const wchar_t* p2, std::wstring* pDupPath = NULL);
std::wstring stdPathCanonicalize(const wchar_t* pPath);

bool checkDupPaths(const std::vector<std::wstring>& saPaths,
    std::wstring& left,
    std::wstring& right,
    std::wstring& common);
#else
    std::u16string stdPathCanonicalize(const char16_t* pPath);
#endif

} }
