#include "stdafx.h"

#include <windows.h>
#include <string>

#include "../stlsoft/include/platformstl/synch/process_mutex.hpp"

void testStlMutex()
{
	stlsoft::platformstl_project::process_mutex pm;
	pm.lock();
}