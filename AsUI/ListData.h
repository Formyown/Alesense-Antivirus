#ifndef _LIST_DATA_H_
#define _LIST_DATA_H_
#include <windows.h>

#define MAX_VIRUS_NAME 35
#define MAX_VIRUS_TYPE 8
#define MAX_ENGINE_NAME 16


typedef struct SCANNING_RES_LIST_ITEM_
{
	struct SCANNING_RES_LIST_ITEM_* Prev;
	struct SCANNING_RES_LIST_ITEM_* Next;

	WCHAR Path[MAX_PATH];
	WCHAR Name[MAX_VIRUS_NAME];
	WCHAR Type[MAX_VIRUS_TYPE];
	WCHAR EngineName[MAX_ENGINE_NAME];

	DWORD Selected;
	DWORD Trusted;
	PVOID Relevance;

}SCANNING_RES_LIST_ITEM, *PSCANNING_RES_LIST_ITEM;

typedef struct SCANNING_RES_LIST_
{
	DWORD Count;
	PSCANNING_RES_LIST_ITEM Head;
	PSCANNING_RES_LIST_ITEM Tail;
	PSCANNING_RES_LIST_ITEM Now;
	DWORD NowIndex;
	CRITICAL_SECTION CriticalSection;
}SCANNING_RES_LIST, *PSCANNING_RES_LIST;




void InitScanningResList(PSCANNING_RES_LIST List);

void DestroyScanningResList(PSCANNING_RES_LIST List);

void ClearScanningResList(PSCANNING_RES_LIST List);

DWORD GetScanningResListItemCount(PSCANNING_RES_LIST List);

PSCANNING_RES_LIST_ITEM GetScanningResListItem(PSCANNING_RES_LIST List, DWORD Index);

BOOL AddScanningResListItem(PSCANNING_RES_LIST List, PWCHAR Path, PWCHAR Name, PWCHAR Type, PWCHAR EngineName, DWORD Trusted, BOOL Selected, PVOID Relevance);

void DeleteScanningResListItem(PSCANNING_RES_LIST List, DWORD Index);

#endif