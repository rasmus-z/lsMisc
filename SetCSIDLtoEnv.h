#pragma once

void SetCSIDLtoEnvW(LPCWSTR pPrefix=NULL, LPCWSTR pSuffix=NULL, std::vector<wstring>* pResult=NULL);

#ifdef UNICODE
#define SetCSIDLtoEnv SetCSIDLtoEnvW
#else
#define SetCSIDLtoEnv SetCSIDLtoEnvA
#endif