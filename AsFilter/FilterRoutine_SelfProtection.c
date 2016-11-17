#include "FilterRoutine.h"
#include "FilterFrame.h"
DWORD32 ProtectedProcessId[MAX_PROCESS] = {0};
DWORD32 ProtectedProcessCount = 0;
DWORD32 ShutdownRoutine = 0x10;//win7 NoParamCall关机参数 win8 = 17
NTSTATUS HookNtTerminateProcess( HANDLE ProcessHandle,__in NTSTATUS ExitStatus)
{
	DWORD32 TargetPID;
	DWORD32 TransferPID;
	NTSTATUS Status;
	PEPROCESS pEProcess;
	Status = ObReferenceObjectByHandle(ProcessHandle, FILE_READ_DATA, NULL, KernelMode,(PVOID*)&pEProcess, NULL);
	if(!NT_SUCCESS(Status))goto RealCase;
	TargetPID = (DWORD32)PsGetProcessId(pEProcess);
	TransferPID = (DWORD32)PsGetCurrentProcessId();
	if(!IsSafeTransferation(TargetPID,TransferPID))
	return STATUS_ACCESS_DENIED;
RealCase:
	return RealNtTerminateProcess(ProcessHandle, ExitStatus);
}
NTSTATUS HookNtOpenProcess ( PHANDLE ProcessHandle,  ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientId)
{
	DWORD32 TargetPID;
	DWORD32 TransferPID;
	NTSTATUS Status;
	PEPROCESS pEProcess;
	Status = RealNtOpenProcess(ProcessHandle,DesiredAccess,ObjectAttributes,ClientId);
	if(NT_SUCCESS(Status))
	{
	TargetPID = (DWORD32)ClientId->UniqueProcess;
	TransferPID = (DWORD32)PsGetCurrentProcessId();
	if(!IsSafeTransferation(TargetPID,TransferPID))
		{
			NtClose(*ProcessHandle);
			*ProcessHandle = NULL;
			return STATUS_ACCESS_DENIED;
		}
	}
RealCase:
	return Status; 
}

NTSTATUS HookNtUserCallOneParam(DWORD32 Param, DWORD32 Routine)
{
	if (Routine == 0x34)
		IsOfflineMode = TRUE;
	return RealNtUserCallOneParam(Param, Routine);
}
DWORD32 List[65536]={0};
NTSTATUS HookNtUserCallNoParam(DWORD32 Routine)
{
	if(Routine == ShutdownRoutine)//win8
		IsOfflineMode = TRUE;
	
	return RealNtUserCallNoParam(Routine);
}



BOOLEAN InsertProtectedProcess(DWORD32 TargetPID)
{
	int i = 0;
	for (;i < MAX_PROCESS; i++)
	{
		if(ProtectedProcessId[i] == 0)
		{
			ProtectedProcessId[i] = TargetPID;
			ProtectedProcessCount ++;
			return TRUE;
		}
	}
	return FALSE;
}
BOOLEAN RemoveProtectedProcess(DWORD32 TargetPID)
{
	int i = 0;
	for (; i < MAX_PROCESS; i++)
	{
		if(ProtectedProcessId[i] == TargetPID)
		{
			ProtectedProcessId[i] = 0;
			ProtectedProcessCount --;
			return TRUE;
		}
	}
	return FALSE;
}
BOOLEAN IsSafeTransferation(DWORD32 TargetPID,DWORD32 TransferPID)
{
	int i = 0;
	for (;i < ProtectedProcessCount; i++)
	{
		if(ProtectedProcessId[i] == TargetPID)
			if(IsSafeProcess(TransferPID))
				return TRUE;
			else
				return FALSE;
	}
	return TRUE;
}
BOOLEAN IsSafeProcess(DWORD32 TransferPID)
{
	int i = 0;
	for (; i < ProtectedProcessCount; i++)
	{
		if(ProtectedProcessId[i] == TransferPID)
			return TRUE;
	}
	return FALSE;
}