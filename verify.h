#ifndef VERIFY
	#ifdef _DEBUG
		#include <assert.h>
		#define VERIFY assert
	#else
		#define VERIFY(f) (f)
	#endif
#endif

