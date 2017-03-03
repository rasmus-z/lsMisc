#include "stdafx.h"
#include <assert.h>
#include <vcclr.h>
#include <string>

#include "getStdString.net.h"

namespace Ambiesoft {

	std::string getStdString(System::String^ s)
	{
		std::string ret;
		if ( s == nullptr )
			return ret;

		pin_ptr<const wchar_t> p = PtrToStringChars(s);
		size_t len = (s->Length+1) * sizeof(wchar_t);
		char* pT = (char*)malloc(len);
		size_t retutrnvalue;

		if ( 0 != wcstombs_s( &retutrnvalue, 
			pT,
			len,
			p,
			len ) )
		{
			assert(false);
			free(pT);
			return ret;
		}
		ret = pT;
		free(pT);
		return ret;
	}


	std::wstring getStdWstring(System::String^ s)
	{
		std::wstring ret;
		if ( s == nullptr )
			return ret;

		pin_ptr<const wchar_t> p = PtrToStringChars(s);
		ret = p;
		return ret;
	}
}