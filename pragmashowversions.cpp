#include "stdafx.h"



#define STRING2(x) #x
#define STRING(x) STRING2(x)
#pragma message("WINVER        : " STRING(WINVER))
#pragma message("_WIN32_WINNT  : " STRING(_WIN32_WINNT))
//  #pragma message("_WIN32_WINDOWS: " STRING(_WIN32_WINDOWS)) // Windows9x only
#pragma message("_WIN32_IE     : " STRING(_WIN32_IE))
#pragma message("_MSC_VER      : " STRING(_MSC_VER))

#ifdef UNICODE
#pragma message("UNICODE defined")
#else
#pragma message("UNICODE not defined")
#endif



