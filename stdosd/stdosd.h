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

#include <vector>
#include <string>

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <wctype.h>

namespace Ambiesoft {
	namespace stdosd {

#if _MSC_VER <= 1800  // less than or equal to VC2013 ( or VC12 )
#define STDOSD_CONSTEXPR const
#else
#define STDOSD_CONSTEXPR const constexpr
#endif

		template<typename C>
		struct stdLiterals
		{
			// static STDOSD_CONSTEXPR C period = '.';
		};
		template<>
		struct stdLiterals<char>
		{
			static STDOSD_CONSTEXPR char period = '.';
			static STDOSD_CONSTEXPR char N0 = '0';
			static STDOSD_CONSTEXPR char N1 = '1';
			static STDOSD_CONSTEXPR char N2 = '2';
			static STDOSD_CONSTEXPR char N3 = '3';
			static STDOSD_CONSTEXPR char N4 = '4';
			static STDOSD_CONSTEXPR char N5 = '5';
			static STDOSD_CONSTEXPR char N6 = '6';
			static STDOSD_CONSTEXPR char N7 = '7';
			static STDOSD_CONSTEXPR char N8 = '8';
			static STDOSD_CONSTEXPR char N9 = '9';

			static STDOSD_CONSTEXPR char* pathSeparators() {
				return "/\\";
			}
			static STDOSD_CONSTEXPR char* nulString() {
				return "";
			}
			static STDOSD_CONSTEXPR char* CR() {
				return "\r";
			}
			static STDOSD_CONSTEXPR char* LF() {
				return (const char*)"\n";
			}
			static STDOSD_CONSTEXPR char* CRLF() {
				return "\r\n";
			}
		};
		template<>
		struct stdLiterals<wchar_t>
		{
			static STDOSD_CONSTEXPR wchar_t period = L'.';
			static STDOSD_CONSTEXPR wchar_t N0 = L'0';
			static STDOSD_CONSTEXPR wchar_t N1 = L'1';
			static STDOSD_CONSTEXPR wchar_t N2 = L'2';
			static STDOSD_CONSTEXPR wchar_t N3 = L'3';
			static STDOSD_CONSTEXPR wchar_t N4 = L'4';
			static STDOSD_CONSTEXPR wchar_t N5 = L'5';
			static STDOSD_CONSTEXPR wchar_t N6 = L'6';
			static STDOSD_CONSTEXPR wchar_t N7 = L'7';
			static STDOSD_CONSTEXPR wchar_t N8 = L'8';
			static STDOSD_CONSTEXPR wchar_t N9 = L'9';

			static STDOSD_CONSTEXPR wchar_t* pathSeparators() {
				return L"/\\";
			}
			static STDOSD_CONSTEXPR wchar_t* nulString() {
				return L"";
			}
			static STDOSD_CONSTEXPR wchar_t* CR() {
				return L"\r";
			}
			static STDOSD_CONSTEXPR wchar_t* LF() {
				return L"\n";
			}
			static STDOSD_CONSTEXPR wchar_t* CRLF() {
				return L"\r\n";
			}

		};
		template<typename C>
		inline bool isEmptyString(const C* str, size_t len) {
			return (len == 0 || !str || str[0] == 0);
		}
		inline size_t getCharLength(const char* p) {
			return std::strlen(p);
		}
		inline size_t getCharLength(const wchar_t* p) {
			return wcslen(p);
		}

#ifdef _MSC_VER
		static int myStrCaseCmp(const char* left, const char* right) {
			return _strcmpi(left, right);
		}
		static int myStrCaseCmpW(const wchar_t* left, const wchar_t* right) {
			return _wcsicmp(left, right);
		}
		static int myStrNCaseCmp(const char* left, const char* right, size_t len) {
			return _strnicmp(left, right, len);
		}
		static int myStrNCaseCmpW(const wchar_t* left, const wchar_t* right, size_t len) {
			return _wcsnicmp(left, right, len);
		}
#else
		static int myStrCaseCmp(const char* left, const char* right) {
			return strcasecmp(left, right);
		}
		static int myStrCaseCmpW(const wchar_t* left, const wchar_t* right) {
			return wcscasecmp(left, right);
		}
		static int myStrNCaseCmp(const char* left, const char* right, size_t len) {
			return strncasecmp(left, right, len);
		}
		static int myStrNCaseCmpW(const wchar_t* left, const wchar_t* right, size_t len) {
			return wcsncasecmp(left, right, len);
		}
#endif

		inline bool isCharEqual(const char* left, const char* right, bool ignoreCase=false) {
			// Linux GNU C dose not have _strcmpi
			return ignoreCase ? myStrCaseCmp(left,right)==0 : strcmp(left, right) == 0;
		}
		inline bool isCharEqual(const wchar_t* left, const wchar_t* right, bool ignoreCase = false) {
			return ignoreCase ? myStrCaseCmpW(left,right)==0 : wcscmp(left, right) == 0;
		}

		inline bool isCharEqualN(const char* left, const char* right, size_t len, bool ignoreCase = false) {
			return ignoreCase ? myStrNCaseCmp(left, right, len) == 0 : strncmp(left, right, len) == 0;
		}
		inline bool isCharEqualN(const wchar_t* left, const wchar_t* right, size_t len, bool ignoreCase = false) {
			return ignoreCase ? myStrNCaseCmpW(left, right, len) == 0 : wcsncmp(left, right, len) == 0;
		}



		inline bool isAsciiDigit(char c) {
			return ('0' <= c) && (c <= '9');
		}
		inline bool isAsciiDigit(wchar_t c) {
			return (L'0' <= c) && (c <= L'9');
		}


		template<typename C>
		inline bool stdIsAsciiDigit(const C* str, size_t len = -1)
		{
			if (isEmptyString(str, len))
				return false;

			if (len == (size_t)-1)
				len = getCharLength(str);

			for (size_t i = 0; i < len; ++i)
			{
				if (!isAsciiDigit(str[i]))
					return false;
			}
			return true;
		}
		inline bool stdIsAsciiDigit(const std::wstring& s)
		{
			return stdIsAsciiDigit(s.c_str(), s.size());
		}
		inline bool stdIsAsciiDigit(const std::string& s)
		{
			return stdIsAsciiDigit(s.c_str(), s.size());
		}





		inline bool isTdigit(char c)
		{
			return !!isdigit(c);
		}
		inline bool isTdigit(wchar_t c)
		{
			return !!iswdigit(c);
		}
		template<typename C>
		inline bool stdIsTdigit(const C* str, size_t len = -1)
		{
			if (isEmptyString(str, len))
				return false;

			if (len == (size_t)-1)
				len = getCharLength(str);

			for (size_t i = 0; i < len; ++i)
			{
				if (!isTdigit(str[i]))
					return false;
			}
			return true;
		}
		inline bool stdIsTdigit(const std::wstring& s)
		{
			return stdIsTdigit(s.c_str(), s.size());
		}
		inline bool stdIsTdigit(const std::string& s)
		{
			return stdIsTdigit(s.c_str(), s.size());
		}

	

		inline const wchar_t* getRChar(const wchar_t* p, wchar_t c)
		{
			return wcsrchr(p, c);
		}
		inline const char* getRChar(const char* p, char c)
		{
			return strrchr(p, c);
		}

		template<typename C>
		inline const C* getOneofRChar(const C* pOriginal, const C* pOneof)
		{
			const C* pRet = nullptr;
			// const C* pOriginal = pOneof;
			for (const C* pC = pOneof; *pC; ++pC)
			{
				const C* pT = getRChar(pOriginal, *pC);
				if (pT)
					++pT;
				if (pT > pRet)
					pRet = pT;
			}
			return pRet;
		}

		template<typename C>
		inline const C* stdGetFileName(const C* pPath)
		{
			if (!pPath)
				return nullptr;
			const C* pSeparator = getOneofRChar(pPath, stdLiterals<C>::pathSeparators());
			if (!pSeparator)
				return pPath;

			return pSeparator;
		}
		inline std::string stdGetFileName(const std::string& w)
		{
			return stdGetFileName(w.c_str());
		}
		inline std::wstring stdGetFileName(const std::wstring& w)
		{
			return stdGetFileName(w.c_str());
		}

		template<typename C>
		inline const C* stdGetFileExtension(const C* pPath)
		{
			if (!pPath)
				return nullptr;

			const C* pFilename = stdGetFileName(pPath);
			if (!pFilename)
				return nullptr;

			const C* pExt = getRChar(pFilename, stdLiterals<C>::period);
			if (!pExt)
				return stdLiterals<C>::nulString();

			return pExt;
		}
		inline std::string stdGetFileExtension(const std::string& w)
		{
			return stdGetFileExtension(w.c_str());
		}
		inline std::wstring stdGetFileExtension(const std::wstring& w)
		{
			return stdGetFileExtension(w.c_str());
		}


		template<typename C>
		inline std::basic_string<C, std::char_traits<C>, std::allocator<C>> 
			stdGetFileNameWitoutExtension(const C* pPath)
		{
			if (!pPath)
				return std::basic_string<C, std::char_traits<C>, std::allocator<C>>();

			const C* pFilename = stdGetFileName(pPath);
			if (!pFilename)
				return std::basic_string<C, std::char_traits<C>, std::allocator<C>>();

			const C* pExt = getRChar(pFilename, stdLiterals<C>::period);
			if (!pExt)
				return pFilename;

			std::basic_string<C, std::char_traits<C>, std::allocator<C>> ret;
			size_t len = pExt - pFilename;
			ret.assign(pFilename, len);
			return ret;
		}
		inline std::string stdGetFileNameWitoutExtension(const std::string& w)
		{
			return stdGetFileNameWitoutExtension(w.c_str());
		}
		inline std::wstring stdGetFileNameWitoutExtension(const std::wstring& w)
		{
			return stdGetFileNameWitoutExtension(w.c_str());
		}



		template<typename StringType>
		inline StringType stdStringReplaceHelper(
			StringType str,
			const StringType& from,
			const StringType& to)
		{
			size_t start_pos = 0;
			while ((start_pos = str.find(from, start_pos)) != StringType::npos)
			{
				str.replace(start_pos, from.length(), to);
				start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
			}
			return str;
		}
		inline std::string stdStringReplace(
			std::string str,
			const std::string& from,
			const std::string& to)
		{
			return stdStringReplaceHelper(str, from, to);
		}
		inline std::wstring stdStringReplace(
			std::wstring str,
			const std::wstring& from,
			const std::wstring& to)
		{
			return stdStringReplaceHelper(str, from, to);
		}




		// https://stackoverflow.com/a/13172514
		template<typename StringType>
		inline std::vector<StringType> stdSplitStringHelper(
			const StringType& str,
			const StringType& delimiter)
		{
			std::vector<StringType> strings;

			size_t pos = 0;
			size_t prev = 0;
			while ((pos = str.find(delimiter, prev)) != StringType::npos)
			{
				strings.push_back(str.substr(prev, pos - prev));
				prev = pos + 1;
			}

			// To get the last substring (or only, if delimiter is not found)
			strings.push_back(str.substr(prev));

			return strings;
		}
		inline std::vector<std::string> stdSplitString(
			const std::string& str,
			const std::string& delimiter)
		{
			return stdSplitStringHelper(str, delimiter);
		}
        inline std::vector<std::wstring> stdSplitString(
            const std::wstring& str,
            const std::wstring& delimiter)
        {
            return stdSplitStringHelper(str, delimiter);
        }
        inline std::vector<std::u16string> stdSplitString(
            const std::u16string& str,
            const std::u16string& delimiter)
        {
            return stdSplitStringHelper(str, delimiter);
        }


		template<typename StringType>
		inline std::vector<StringType> stdSplitStringToLineHelper(const StringType& str)
		{
			using C = typename StringType::traits_type::char_type;

			StringType t = stdStringReplace(
				stdStringReplace(
					str,
					stdLiterals<C>::CRLF(),
					stdLiterals<C>::LF()),
				stdLiterals<C>::CR(),
				stdLiterals<C>::LF());

			return stdSplitString(t, stdLiterals<C>::LF());
		}
		inline std::vector<std::string> stdSplitStringToLine(const std::string& str)
		{
			return stdSplitStringToLineHelper(str);
		}
		inline std::vector<std::wstring> stdSplitStringToLine(const std::wstring& str)
		{
			return stdSplitStringToLineHelper(str);
		}


#ifndef __cplusplus_cli
		inline int stdSprintF(char *buffer,
							  size_t sizeOfBuffer,
							  size_t count,
							  const char *format,
							  va_list argptr)
		{
#if defined(__MINGW32__)

            // (void)count;
            return vsnprintf_s(buffer, sizeOfBuffer, count, format, argptr);
#elif defined(__GNUC__)
            return vsnprintf(buffer, sizeOfBuffer, format, argptr);

#elif _WIN32
#if _MSC_VER <= 1400
			int n = _vsnwprintf((wchar_t *)str.data(), size, fmt.c_str(), ap);
#else
			return _vsnprintf_s(buffer,sizeOfBuffer,count,format,argptr);
#endif // _WIN32
#endif // __GUNC__
		}
		inline int stdSprintF(wchar_t *buffer,
							  size_t sizeOfBuffer,
							  size_t count,
							  const wchar_t *format,
							  va_list argptr)
		{
#if defined(__MINGW32__)
            (void)count;
            return vswprintf_s(buffer, sizeOfBuffer, format, argptr);
#elif defined(__GNUC__)
            return vswprintf(buffer,sizeOfBuffer,format,argptr);

#elif _WIN32
#if _MSC_VER <= 1400
			int n = _vsnwprintf((wchar_t *)str.data(), size, fmt.c_str(), ap);
#else
			return _vsnwprintf_s(buffer,sizeOfBuffer,count,format,argptr);
#endif // _WIN32
#endif // __GUNC__
		}


		template<typename StringType>
		inline StringType stdFormatHelper(const StringType fmt, ...)
		{
			using C = typename StringType::traits_type::char_type;

			int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
			StringType str;
			va_list ap;
			while (1) {     // Maximum two passes on a POSIX system...
				str.resize(size);
				va_start(ap, fmt);

				int n = stdSprintF((C *)str.data(), size, size - 1, fmt.c_str(), ap);

				va_end(ap);
				if (n > -1 && n < size) {  // Everything worked
					str.resize(n);
					return str;
				}
				if (n > -1)  // Needed size returned
					size = n + 1;   // For null char
				else
					size *= 2;      // Guess at a larger size (OS specific)
			}
			return str;
		}
		template<typename... ARGS>
		inline std::string stdFormat(const std::string& fmt, ARGS... args)
		{
			return stdFormatHelper(fmt, args...);
		}
		template<typename... ARGS>
		inline std::wstring stdFormat(const std::wstring& fmt, ARGS... args)
		{
			return stdFormatHelper(fmt, args...);
		}
#endif // __cplusplus_cli








		template<typename C>
		inline bool hasEnding(const C* fullString, const C* ending, 
			size_t fullLen = -1, size_t endLen = -1,
			bool ignoreCase = false) {
			if (fullString == nullptr)
				return false;
			
			if (ending == nullptr || ending[0] == 0)
				return true;

			if (fullLen == (size_t)-1)
				fullLen = getCharLength(fullString);
			if (endLen == (size_t)-1)
				endLen = getCharLength(ending);

			if (fullLen < endLen)
				return false;
			
			const C* pFullStartPos = fullString + (fullLen - endLen);
			return isCharEqual(pFullStartPos, ending, ignoreCase);
		}
		template<typename C>
		inline bool hasEndingI(const C* fullString, const C* ending,
			size_t fullLen = -1, size_t endLen = -1) {
			return hasEnding(fullString, ending,
				fullLen, endLen, true);
		}
		inline bool hasEnding(const std::string& fullString, const std::string& ending) {
			return hasEnding(
				fullString.c_str(),
				ending.c_str(),
				fullString.size(),
				ending.size()
			);
		}
		inline bool hasEndingI(const std::string& fullString, const std::string& ending) {
			return hasEndingI(
				fullString.c_str(),
				ending.c_str(),
				fullString.size(),
				ending.size()
			);
		}
		inline bool hasEnding(const std::wstring& fullString, const std::wstring& ending) {
			return hasEnding(
				fullString.c_str(),
				ending.c_str(),
				fullString.size(),
				ending.size()
			);
		}
		inline bool hasEndingI(const std::wstring& fullString, const std::wstring& ending) {
			return hasEndingI(
				fullString.c_str(),
				ending.c_str(),
				fullString.size(),
				ending.size()
			);
		}


		template<typename C>
		inline bool hasStarting(const C* fullString, const C* starting,
			size_t fullLen = -1, size_t startLen = -1,
			bool ignoreCase = false) {
			if (fullString == nullptr)
				return false;

			if (starting == nullptr || starting[0] == 0)
				return true;

			if (fullLen == (size_t)-1)
				fullLen = getCharLength(fullString);
			if (startLen == (size_t)-1)
				startLen = getCharLength(starting);

			if (fullLen < startLen)
				return false;

			return isCharEqualN(fullString, starting, startLen, ignoreCase);
		}
		template<typename C>
		inline bool hasStartingI(const C* fullString, const C* starting,
			size_t fullLen = -1, size_t startLen = -1) {
			return hasStarting(fullString, starting,
				fullLen, startLen, true);
		}
		inline bool hasStarting(const std::string& fullString, const std::string& starting) {
			return hasStarting(
				fullString.c_str(),
				starting.c_str(),
				fullString.size(),
				starting.size()
			);
		}
		inline bool hasStartingI(const std::string& fullString, const std::string& starting) {
			return hasStartingI(
				fullString.c_str(),
				starting.c_str(),
				fullString.size(),
				starting.size()
			);
		}
		inline bool hasStarting(const std::wstring& fullString, const std::wstring& starting) {
			return hasStarting(
				fullString.c_str(),
				starting.c_str(),
				fullString.size(),
				starting.size()
			);
		}
		inline bool hasStartingI(const std::wstring& fullString, const std::wstring& starting) {
			return hasStartingI(
				fullString.c_str(),
				starting.c_str(),
				fullString.size(),
				starting.size()
			);
		}


		std::wstring stdGetFullPathName(const std::wstring& ws);

		std::wstring resolveLink(const std::wstring& fullstring);
	}
}
