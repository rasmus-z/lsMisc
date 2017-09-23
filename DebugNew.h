#pragma once

#ifdef _DEBUG
#include <windows.h>
#include <crtdbg.h>
#define malloc(size) _malloc_dbg(size, _NORMAL_BLOCK, __FILE__, __LINE__ )
#define calloc(s1,s2) _calloc_dbg(s1, s2, _NORMAL_BLOCK, __FILE__, __LINE__ )
#define realloc(p,s) _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define free(p)  _free_dbg(p, _NORMAL_BLOCK)
inline void* operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
{
 return _malloc_dbg(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
}
inline void operator delete(void *p)
{
 _free_dbg(p, _NORMAL_BLOCK);
}
#define DEBUG_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#define new DEBUG_NEW
#endif