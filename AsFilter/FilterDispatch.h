#ifndef _FILTER_DISPATCH_H_
#define _FILTER_DISPATCH_H_
#include "FilterFrame.h"

void HandleEvent(PKEVENT Event, PKEVENT EventWaitList, PVOID PMsg, PLIST_ENTRY List, PKSPIN_LOCK Lock, PDWORD32 Length);

///**about TerminateProcess
typedef struct _TP_MSG
{
	LIST_ENTRY ListEntry;
	DWORD32 Pointer;
	PKEVENT Event;
	HANDLE Source;
	BOOLEAN IsPass;
	HANDLE Target;
}*PTP_MSG, TP_MSG;
LIST_ENTRY List_TP;
KSPIN_LOCK Lock_TP;
DWORD32 Length_TP;
KEVENT Event_TP_Wait_List;
///**
///**about LoadSection
typedef struct _LM_MSG
{
	LIST_ENTRY ListEntry;
	DWORD32 Pointer;
	PKEVENT Event;
	HANDLE Source;
	BOOLEAN IsPass;
	DWORD32 PatahLength;
	WCHAR TargetPath[1024];
}*PLM_MSG, LM_MSG;
LIST_ENTRY List_LM;
KSPIN_LOCK Lock_LM;
DWORD32 Length_LM;
KEVENT Event_LM_Wait_List;
///**
///**about CreateProcess
typedef struct _CP_MSG
{
	LIST_ENTRY ListEntry;
	DWORD32 Pointer;
	PKEVENT Event;
	HANDLE Source;
	BOOLEAN IsPass;
	DWORD32 PatahLength;
	WCHAR TargetPath[1024];
}*PCP_MSG, CP_MSG;
LIST_ENTRY List_CP;
KSPIN_LOCK Lock_CP;
DWORD32 Length_CP;
KEVENT Event_CP_Wait_List;
///**
///**about DriverOpreation
typedef enum _DV_MSGTYPE
{
	LoadDriver,
	UnloadDriver
}DV_MSGTYPE;
typedef struct _DV_MSG
{
	LIST_ENTRY ListEntry;
	DWORD32 Pointer;
	PKEVENT Event;
	HANDLE Source;
	BOOLEAN IsPass;
	DWORD32 MsgType;
	DWORD32 PatahLength;
	WCHAR TargetPath[1024];
}*PDV_MSG, DV_MSG;
LIST_ENTRY List_DV;
KSPIN_LOCK Lock_DV;
DWORD32 Length_DV;
KEVENT Event_DV_Wait_List;
///**
///**about SetValueKey

typedef struct _SK_MSG
{
	LIST_ENTRY ListEntry;
	DWORD32 Pointer;
	PKEVENT Event;
	HANDLE Source;
	BOOLEAN IsPass;
	DWORD32 ValueType;
	DWORD32 PatahLength;
	DWORD32 DataLength;
	WCHAR TargetPathAndValue[1024];
}*PSK_MSG, SK_MSG;
LIST_ENTRY List_SK;
KSPIN_LOCK Lock_SK;
DWORD32 Length_SK;
KEVENT Event_SK_Wait_List;
///**
///**
///**about AllocVirualMemory
typedef struct _AM_MSG
{
	LIST_ENTRY ListEntry;
	DWORD32 Pointer;
	PKEVENT Event;
	HANDLE Source;
	BOOLEAN IsPass;
	DWORD32 Target;
}*PAM_MSG, AM_MSG;
LIST_ENTRY List_AM;
KSPIN_LOCK Lock_AM;
DWORD32 Length_AM;
KEVENT Event_AM_Wait_List;
///**
///**
///**about OpenProcess
typedef struct _OP_MSG
{
	LIST_ENTRY ListEntry;
	DWORD32 Pointer;
	PKEVENT Event;
	HANDLE Source;
	BOOLEAN IsPass;
	DWORD32 Target;
}*POP_MSG, OP_MSG;
LIST_ENTRY List_OP;
KSPIN_LOCK Lock_OP;
DWORD32 Length_OP;
KEVENT Event_OP_Wait_List;
///**

#endif