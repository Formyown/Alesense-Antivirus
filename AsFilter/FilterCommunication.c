#include "FilterCommunication.h"
#include "FilterRoutine.h"
#include <wchar.h>
#include <Ntstrsafe.h>

KEVENT DriverEventSyn;
BOOLEAN IsOfflineMode = TRUE; //是否是下线状态
LIST_ENTRY EventQueueEntry;
KSPIN_LOCK EventQueueSpinLock;
DWORD32 EventAmount = 0;
void InitlizeCommunication()
{
	KeInitializeEvent(&DriverEventSyn,SynchronizationEvent,1);
	InitializeListHead(&EventQueueEntry);
	
	KeInitializeSpinLock(&EventQueueSpinLock);
}
BOOLEAN PassEvent(LPWSTR Content,LPWSTR SubContent,DWORD32 MsgType,DWORD32 SubValue,DWORD32 Value,DWORD32 NeedScan)
{
	DWORD32 Result = -1;//表示无信号
	LARGE_INTEGER Delay = {0};	//Sleep
	DRIVEREVENTQUEUE EventQueue = {0};
	KeWaitForSingleObject(&DriverEventSyn,Executive,KernelMode,0,0);
	if(IsOfflineMode == TRUE) //如果这时候下线
	{
	KeSetEvent(&DriverEventSyn,0,0);
	return TRUE;
	}
	
	if(Content != NULL)
		RtlStringCbCopyW(EventQueue.Event.Content,MAX_CONTENT*sizeof(WCHAR),Content);
	if(SubContent != NULL)
		RtlStringCbCopyW(EventQueue.Event.SubContent,MAX_CONTENT*sizeof(WCHAR),SubContent);
	EventQueue.Event.MessageType = MsgType;
	EventQueue.Event.SubValue = SubValue;
	EventQueue.Event.Value = Value;
	EventQueue.Event.NeedScan = NeedScan;
	EventQueue.Event.DrvPtr = (DWORD32)&Result;
	ExInterlockedInsertTailList(&EventQueueEntry,&EventQueue.ListEntry,&EventQueueSpinLock);
	EventAmount++;
	KeSetEvent(&DriverEventSyn,0,0);
	Delay.QuadPart = -10 * 1000 ;
	while(1)
	{
		KeDelayExecutionThread(KernelMode,0,&Delay);
		if(IsOfflineMode == TRUE)
		{
			return TRUE;
		}
		if(Result != -1)
		{
			return Result;
		}
	}
	return Result;
}
NTSTATUS IoControl(PDEVICE_OBJECT DriverObject,PIRP Irp)
{
	PIO_STACK_LOCATION Irpsp = IoGetCurrentIrpStackLocation(Irp);
	DWORD32 Code = Irpsp->Parameters.DeviceIoControl.IoControlCode;
	DWORD32 In_Size = Irpsp->Parameters.DeviceIoControl.InputBufferLength;
	DWORD32 Out_Size = Irpsp->Parameters.DeviceIoControl.OutputBufferLength;
	PVOID Buffer = Irp->AssociatedIrp.SystemBuffer;
	NTSTATUS Status;
	DWORD32 Address = 0;
	Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
	Irp->IoStatus.Information = 0;
	//================单条消息====================//
	if(Code == CTL_WAITFOREVENT)					//MemoryShare
	{	
		PDRIVEREVENTQUEUE Queue;
			if(EventAmount == 0)
				goto Exit;
		Queue = (PDRIVEREVENTQUEUE)ExInterlockedRemoveHeadList(&EventQueueEntry,&EventQueueSpinLock);
		EventAmount--;
		if(Buffer)
		{
			RtlMoveMemory(Buffer,&Queue->Event,sizeof(DRIVEREVENTMSG));
			Irp->IoStatus.Status = STATUS_SUCCESS; 
			Irp->IoStatus.Information = sizeof(DRIVEREVENTMSG);
		
		}
		
	}
	if(Code == CTL_INSERTPROTECTPID)				
	{
		InsertProtectedProcess(*((PDWORD32)Buffer));
		Irp->IoStatus.Status = STATUS_SUCCESS;   
		Irp->IoStatus.Information = sizeof(DWORD32);
	}
	if(Code == CTL_REMOVEPROTECTPID)				
	{
		RemoveProtectedProcess(*((PDWORD32)Buffer));
		Irp->IoStatus.Status = STATUS_SUCCESS;   
		Irp->IoStatus.Information = sizeof(DWORD32);
	}
	if(Code == CTL_EVENTCALLBACK)
	{
		PDRIVERCALLBACKMSG CallBack = (PDRIVERCALLBACKMSG)Buffer;
		__try
		{
			if(MmIsAddressValid((PVOID)CallBack->DrvPtr))
			{
			*((PDWORD32)CallBack->DrvPtr) = CallBack->Result;
			Irp->IoStatus.Status = STATUS_SUCCESS;   
			Irp->IoStatus.Information = sizeof(DRIVERCALLBACKMSG);
			}
		}
		__except(1)
		{
			goto Exit;
		}
		
	}
	//===========================================//
	if(Code == CTL_STARTPROTECTION)
	{
		IsOfflineMode = FALSE;
		Irp->IoStatus.Status = STATUS_SUCCESS;
	}
	if(Code == CTL_STOPPROTECTION)
	{
		IsOfflineMode = TRUE;
		Irp->IoStatus.Status = STATUS_SUCCESS;
	}
	//============================================//
	Exit:
	//============================================//	
	IoCompleteRequest(Irp,IO_NO_INCREMENT);
	return Irp->IoStatus.Status;
}
