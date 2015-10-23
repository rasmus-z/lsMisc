void UrlEncode(const unsigned char	*csource,
						size_t	nSize,
						char** pOut,
						int bUseMalloc = 0);

char *urlencodenew( char *pstr );
char *urlencodenew2(char *pstr, size_t size);

unsigned char* urldecode(const char* penc, unsigned int* pSize);