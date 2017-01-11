#pragma once
#include "stlsoft/smartptr/scoped_handle.hpp"



#define STLSCOPEDFREE(ptr) stlsoft::scoped_handle<void*> stlscopedfree_##ptr((void*)ptr, free)