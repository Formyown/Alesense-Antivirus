#include "ListData.h"
#include "LuaOperation.h"
SCANNING_RES_LIST List;

DWORD BufferWindowIndex;

void InitListTransport()
{
	InitScanningResList(&List);
	BufferWindowIndex = 0;
}

void CloseListTransport()
{
	DestroyScanningResList(&List);
	BufferWindowIndex = 0;
}


void InformUI_AddScanningResListItem(LPCWSTR Path, LPCWSTR Name, LPCWSTR Type, LPCWSTR EngineName, PVOID Relevance)
{
	AddScanningResListItem(&List, (PWCHAR)Path, (PWCHAR)Name, (PWCHAR)Type, (PWCHAR)EngineName, FALSE, TRUE, Relevance);
	DWORD Count = GetScanningResListItemCount(&List);
	
	//if (Count <= 25)
	//{
	InformUI_ScanningResultItem(ScanningResultListAddNext, (PWCHAR)Name, (PWCHAR)Path, (PWCHAR)Type, (PWCHAR)EngineName, FALSE, TRUE);
	//}
	InformUI_Int(ScanningResultListSetCount, GetScanningResListItemCount(&List));

}
void InformControl_ScanningResListGetNextItem()
{
	BufferWindowIndex++;
	PSCANNING_RES_LIST_ITEM Item = GetScanningResListItem(&List, BufferWindowIndex + 25);
	if (Item != NULL)
	{
		InformUI_ScanningResultItem(ScanningResultListAddNext, Item->Name, Item->Path, Item->Type, Item->EngineName, Item->Trusted, Item->Selected);
	}
	else
	{
		BufferWindowIndex--;
	}
		
	

}
void InformControl_ScanningResListGetPrevItem()
{
	BufferWindowIndex--;
	PSCANNING_RES_LIST_ITEM Item = GetScanningResListItem(&List, BufferWindowIndex);
	if (Item != NULL)
	{
		InformUI_ScanningResultItem(ScanningResultListAddPrev, Item->Name, Item->Path, Item->Type, Item->EngineName, Item->Trusted, Item->Selected);
	}
	else
	{
		BufferWindowIndex++;
	}
}

DWORD ScanningResListGeCount()
{
	return GetScanningResListItemCount(&List);
}

void ScanningResListClear()
{
	ClearScanningResList(&List);
}
void InformControl_ScanningResListSetTrused(DWORD Index, DWORD Trusted)
{
	PSCANNING_RES_LIST_ITEM Item = GetScanningResListItem(&List, Index);
	if (Item != NULL)
	{
		Item->Trusted = Trusted;
	}
}
void InformControl_ScanningResListSetSelected(DWORD Index, DWORD Selected)
{
	PSCANNING_RES_LIST_ITEM Item = GetScanningResListItem(&List, Index);
	if (Item != NULL)
	{
		Item->Selected = Selected;
	}
}
PSCANNING_RES_LIST_ITEM ScanningResListGeItem(DWORD Index)
{
	return GetScanningResListItem(&List, Index);
}
