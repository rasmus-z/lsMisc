#define NOMINMAX

// Basically gtest leaks, so turn it off
//#ifndef NDEBUG
//// include only in DEBUG mode
//#include <vld.h>
//#endif

#include "../PragmaShowVersions.h"

#include <afxwin.h>
// #include <Windows.h>
#include <Tlhelp32.h>
#include <Psapi.h>
#include <Shlwapi.h>
#include <tchar.h>

#pragma warning(push)
#pragma warning(disable: 4091)
#include <ShlObj.h>
#pragma warning(pop)

#include <cctype>
#include <cstdarg>
#include <functional>
#include <locale>
#include <memory.h>

#include <cassert>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>


// In lsMiscTest directory, run the following command
// git submodule init
// git submodule update
#include "gtest/gtest.h"