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
	assert(RevealFolder(L"P:\\"));
	assert(RevealFolder(L"P:\\AAAA"));
	assert(!RevealFolder(L"P:\\AAAfwefewfewwefewweA"));
}