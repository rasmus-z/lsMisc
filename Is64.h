#pragma once

namespace Ambiesoft {
	bool Is64BitWindows();
	LPCWSTR GetPlatformW();
	bool Is64BitProcess();
}