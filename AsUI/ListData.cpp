#include "ListData.h"

void InitScanningResList(PSCANNING_RES_LIST List)
{
	InitializeCriticalSection(&List->CriticalSection);
	List->Count = 0;
	List->Head = List->Now = List->Tail = NULL;
	List->NowIndex = -1;
}
void DestroyScanningResList(PSCANNING_RES_LIST List)
{
	ClearScanningResList(List);
	DeleteCriticalSection(&List->CriticalSection);
}
void ClearScanningResList(PSCANNING_RES_LIST List)
{
	if (List->Count == 0)
		return;
	EnterCriticalSection(&List->CriticalSection);
	List->Now = List->Head->Next;
	while (List->Count > 1)
	{
		free(List->Head);
		List->Head = List->Now;
		List->Now = List->Now->Next;
		List->Count--;
	}
	free(List->Tail);
	List->Count--;
	LeaveCriticalSection(&List->CriticalSection);
}
DWORD GetScanningResListItemCount(PSCANNING_RES_LIST List)
{
	return List->Count;
}
PSCANNING_RES_LIST_ITEM GetScanningResListItem(PSCANNING_RES_LIST List,DWORD Index)
{
	EnterCriticalSection(&List->CriticalSection);
	if (Index > List->Count - 1 || Index < 0)
	{
		LeaveCriticalSection(&List->CriticalSection);
		return NULL;
	}
	if (Index == 0)
	{
		LeaveCriticalSection(&List->CriticalSection);
		return List->Head;
	} 
	else if (Index == List->Count - 1)
	{
		LeaveCriticalSection(&List->CriticalSection);
		return List->Tail;
	}
	else if (Index > List->NowIndex)
	{
		while (Index > List->NowIndex)
		{
			List->Now = List->Now->Next;
			List->NowIndex++;
		}
		LeaveCriticalSection(&List->CriticalSection);
		return List->Now;
	}
	else if (Index < List->NowIndex)
	{
		while (Index < List->NowIndex)
		{
			List->Now = List->Now->Prev;
			List->NowIndex--;
		}
		LeaveCriticalSection(&List->CriticalSection);
		return List->Now;
	}
	else
	{
		LeaveCriticalSection(&List->CriticalSection);
		return List->Now;
	}
}
BOOL AddScanningResListItem(PSCANNING_RES_LIST List, PWCHAR Path, PWCHAR Name, PWCHAR Type, PWCHAR EngineName, DWORD Trusted, BOOL Selected, PVOID Relevance)
{
	EnterCriticalSection(&List->CriticalSection);
	PSCANNING_RES_LIST_ITEM Item = (PSCANNING_RES_LIST_ITEM)malloc(sizeof(SCANNING_RES_LIST_ITEM));
	if (Item == NULL) return FALSE;

	lstrcpy(Item->Name, Name);
	lstrcpy(Item->Path, Path);
	lstrcpy(Item->Type, Type);
	lstrcpy(Item->EngineName, EngineName);
	Item->Trusted = Trusted;
	Item->Selected = Selected;
	Item->Relevance = Relevance;

	if (List->Count != 0)
	{
		List->Now = Item;
		List->Tail->Next = Item;
		Item->Next = NULL;
		Item->Prev = List->Tail;
		List->Tail = Item;
	}
	else
	{
		List->Head = Item;
		List->Tail = Item;
		Item->Prev = NULL;
		Item->Next = NULL;
	}
	

	List->Count++;
	List->NowIndex = List->Count - 1;
	LeaveCriticalSection(&List->CriticalSection);
	return TRUE;
}
void DeleteScanningResListItem(PSCANNING_RES_LIST List, DWORD Index)
{
	PSCANNING_RES_LIST_ITEM Item = GetScanningResListItem(List, Index);
	EnterCriticalSection(&List->CriticalSection);
	Item->Prev->Next = Item->Next;
	Item->Next->Prev = Item->Prev;
	free(Item);
	LeaveCriticalSection(&List->CriticalSection);
}