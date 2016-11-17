
#ifndef _LIST_TRANSPORT_H_
#define _LIST_TRANSPORT_H_
#include <Windows.h>
#include "ListData.h"
void InitListTransport();


void CloseListTransport();

void InformUI_AddScanningResListItem(LPCWSTR Path, LPCWSTR Name, LPCWSTR Type, LPCWSTR EngineName, PVOID Relevance);

void InformControl_ScanningResListGetNextItem();
void InformControl_ScanningResListSetTrused(DWORD Index, DWORD Trusted);
void InformControl_ScanningResListSetSelected(DWORD Index, DWORD Selected);
void ScanningResListClear();
DWORD ScanningResListGeCount();
PSCANNING_RES_LIST_ITEM ScanningResListGeItem(DWORD Index);
#endif