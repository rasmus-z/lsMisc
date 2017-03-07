#pragma once

void UrlEncode_SJIS_Obsolete(const unsigned char	*csource,
						size_t	nSize,
						char** pOut,
						int bUseMalloc = 0);

char *UrlEncode(const unsigned char *pstr, size_t size=-1);
unsigned char* UrlDecode(const char* penc, unsigned int* pSize=NULL);
