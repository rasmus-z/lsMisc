#include <tchar.h>
#include <windows.h>

#include "SkipSpace.h"

LPCTSTR SkipSpace(LPCTSTR p)
{
	while(*p)
	{
		if ( _istspace(*p) )
		{
			p = _tcsinc(p);
			continue;
		}
		return p;
	}
	return p;
}
