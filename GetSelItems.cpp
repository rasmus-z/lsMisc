#include <windows.h>
#include <Commctrl.h>

#include <vector>

#include "GetSelItems.h"

BOOL GetSelItems(HWND hWndList, std::vector<int>& nSelItems)
{
	nSelItems.erase(nSelItems.begin(), nSelItems.end());

	for( int i=0 ; i < SendMessage(hWndList, LVM_GETITEMCOUNT, 0, 0) ; i++ )
	{
		LV_ITEM lvi;
		lvi.mask = LVIF_STATE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.stateMask = 0xFFFF;		
		::SendMessage(hWndList, LVM_GETITEM, 0, (LPARAM)&lvi);
		if (lvi.state & LVIS_SELECTED)
		{
			nSelItems.push_back(i);
		}
	}

	return !nSelItems.empty();
}
