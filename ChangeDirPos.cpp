#include <windows.h>
#include <Commctrl.h>
#include <vector>
using namespace std;
#include "ChangeDirPos.h"
#include "GetSelItems.h"

#include "verify.h"

static BOOL ChangeDirPosStuff( HWND hWndList, int nSelItem, ChangePosType nType )
{
//	int nSelItem = FindSelectedItem(hWndList);
	if( nSelItem == -1 )
		return FALSE;

	if( nType==POS_TO_UP && nSelItem==0 )
		return FALSE;

	if( nType==POS_TO_DOWN && ListView_GetItemCount(hWndList) <= (1+nSelItem) )
		return FALSE;

	int nNewSelItem;
	if( nType==0 )
		nNewSelItem = nSelItem-1;	
	else
		nNewSelItem = nSelItem+1;

	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVITEM));
	lvi.iItem = nSelItem;
	lvi.mask = LVIF_PARAM | LVIF_STATE;
	lvi.stateMask = LVIS_SELECTED;
	VERIFY(SendMessage(hWndList, LVM_GETITEM, 0, (LPARAM)&lvi));

	LVITEM lvi2;
	memset(&lvi2, 0, sizeof(LVITEM));
	lvi2.iItem = nNewSelItem;
	lvi2.mask = LVIF_PARAM | LVIF_STATE;
	lvi.stateMask = LVIS_SELECTED;
	VERIFY(SendMessage(hWndList, LVM_GETITEM, 0, (LPARAM)&lvi2));
	
	// swap it
	lvi .iItem = nNewSelItem;
	lvi.stateMask = LVIS_SELECTED;
	lvi2.iItem = nSelItem;
	lvi2.stateMask = LVIS_SELECTED;
	VERIFY(SendMessage(hWndList, LVM_SETITEM, 0, (LPARAM)&lvi ));
	VERIFY(SendMessage(hWndList, LVM_SETITEM, 0, (LPARAM)&lvi2));

	return TRUE;
}

void ChangeDirPos( HWND hWndList, ChangePosType nType )
{
	std::vector<int> nSelItems;
	GetSelItems(hWndList, nSelItems);

	SendMessage(hWndList, WM_SETREDRAW, FALSE, 0);
	std::vector<int>::iterator it;
	std::vector<int>::reverse_iterator rit;

	if( nType == POS_TO_UP )
	{
		for( it = nSelItems.begin() ;
			 it != nSelItems.end() ;
			 ++it )
		{
			if(!ChangeDirPosStuff(hWndList, *it, nType))
				break;
		}
	}
	else
	{
		for( rit = nSelItems.rbegin() ;
			 rit != nSelItems.rend() ;
			 ++rit )
		{
			if(!ChangeDirPosStuff(hWndList, *rit, nType))
				break;
		}
	}
	SendMessage(hWndList, WM_SETREDRAW, TRUE, 0);
	UpdateWindow(hWndList);
}




