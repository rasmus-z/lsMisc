
#ifndef FREECRT
#define FREECRT(p) \
	do \
	{ \
	if (p) \
		free((void*)p); \
		p = NULL; \
	} while (0) \

#endif

#ifndef _countof
	#define _countof(x) (sizeof(x)/sizeof(x[0]))
#endif
