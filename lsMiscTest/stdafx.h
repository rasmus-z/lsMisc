#define NOMINMAX

#ifndef NDEBUG
// include only in DEBUG mode
#include <vld.h>
#endif

#include <afxwin.h>
// #include <Windows.h>
#include <Tlhelp32.h>
#include <Psapi.h>
#include <Shlwapi.h>

#include <tchar.h>
#include <cassert>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>



#include "gtest/gtest.h"