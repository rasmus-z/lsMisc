#pragma once

namespace Ambiesoft {

	enum MOVEWINDOWTYPE
	{
		MOVEWINDOW_TOPLEFT,
		MOVEWINDOW_TOPRIGHT,
		MOVEWINDOW_BOTTOMLEFT,
		MOVEWINDOW_BOTTOMRIGHT,
	};
	BOOL CenterWindow(HWND hWnd, HWND hwndParent = NULL);
	bool MoveWindowCommon(HWND hWnd, MOVEWINDOWTYPE type);
}