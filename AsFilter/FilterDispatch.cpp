#include "FilterDispatch.h"
/////////////////////////////////////////////////////////////////////////////
//@@@@@@@@@@@@@@@@@@@@@           HandleEvent      @@@@@@@@@@@@@@@@@@@@@@@@//
/////////////////////////////////////////////////////////////////////////////
void HandleEvent(PKEVENT Event, PKEVENT EventWaitList, PVOID PMsg, PLIST_ENTRY List, PKSPIN_LOCK Lock, PDWORD32 Length)
{
	KeInitializeEvent(Event, SynchronizationEvent, FALSE);
	ExInterlockedInsertHeadList(List, (PLIST_ENTRY)PMsg, Lock);
	(*Length)++;
	KeSetEvent(EventWaitList, IO_NO_INCREMENT, FALSE);
	KeWaitForSingleObject(Event, Executive, KernelMode, 0, 0);
}