#include <windows.h>
#include <tchar.h>
#include "tstring.h"

tstring getWindowTstring(HWND h)
{
	tstring ret;
	int len = GetWindowTextLength(h);
	if(len <= 0)
		return ret;

	LPTSTR p = (LPTSTR)LocalAlloc(LMEM_FIXED, (len+2)*sizeof(TCHAR));
	if(!p)
		return ret;

	struct st_pfreer {
		void* p_;
		st_pfreer(void* p) { p_ = p; }
		~st_pfreer() { 
			LocalFree(p_); 
		}
	} pfreer(p);

	int nr = GetWindowText(h, p, len+1);
	if(nr==0)
		return ret;

	ret = p;
	return ret;
}
