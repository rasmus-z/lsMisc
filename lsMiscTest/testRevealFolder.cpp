#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>

#include <cassert>
#include <string.h>
#include <string>

#include "../RevealFolder.h"

#include "debug.h"

using namespace Ambiesoft;

void testRevealFolder()
{
	RevealFolder(L"P:\\");
	RevealFolder(L"P:\\AAAA");
}