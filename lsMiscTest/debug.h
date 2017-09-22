#pragma once
#include <Windows.h>
#include <string>
#include <cassert>

#define DVERIFY(a) assert(a)

void mytrace(LPCWSTR p);
void mytrace(const std::wstring& ss);
