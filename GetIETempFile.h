
#pragma once
inline std::string GetIETempFile()
{
	TCHAR szT[MAX_PATH]; szT[0] = 0;

	if ( ! CreateUrlCacheEntry(
		_T(""), //location,
		0, // location.length(),
		_T("html"),
		szT,
		0) )
	{
		return _T("");
	}

	return szT;
}

		
