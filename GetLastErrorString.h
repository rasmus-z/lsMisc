#include "tstring.h"

std::string GetLastErrorStringA(DWORD dwErrorNo, BOOL* pSeikou=NULL );
std::wstring GetLastErrorStringW(DWORD dwErrorNo, BOOL* pSeikou=NULL );
#ifdef UNICODE
	#define GetLastErrorString GetLastErrorStringW
#else
	#define GetLastErrorString GetLastErrorStringA
#endif
