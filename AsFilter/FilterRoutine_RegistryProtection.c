#include "FilterRoutine.h"
#include "FilterCommunication.h"
#include "FilterFrame.h"
NTSTATUS HookNtSetValueKey(IN HANDLE KeyHandle,IN PUNICODE_STRING ValueName,IN DWORD32 TitleIndex,IN DWORD32 type1,IN PVOID Data,IN DWORD32 DataSize)
{
	 NTSTATUS status=0; 
	 DWORD32 TransferPID; 
	 PVOID pKey;
	 PUNICODE_STRING KeyName;
	 LPWSTR KeyNameBuffer;
	 UNICODE_STRING pData;
	 DWORD32 Status;
	 BOOLEAN Ret = TRUE;
	 __try
	 {
		 ProbeForRead(ValueName,sizeof(UNICODE_STRING),PROBE_ALIGNMENT(UNICODE_STRING));
		 if(DataSize>0)
		 ProbeForRead(Data,DataSize,sizeof(WCHAR));
	 }
	 __except(1)
	 {
		return STATUS_INVALID_PARAMETER;
	 }			
	 KeyName =(PUNICODE_STRING) ExAllocatePool(NonPagedPool, 1024);	
	 if(KeyName == NULL)
		 goto RealCase;
	 TransferPID = (DWORD32)PsGetCurrentProcessId();												
	 status=ObReferenceObjectByHandle(KeyHandle, 0, 0, KernelMode, &pKey, NULL);	
	 if(!NT_SUCCESS(status))goto RealCase;																
	 status=ObQueryNameString(pKey, (POBJECT_NAME_INFORMATION)KeyName, 1024, &Status);				
	 if(!NT_SUCCESS(status)) goto RealCase;				
	 KeyNameBuffer = KeyName->Buffer;
	 if(ValueName->Buffer == NULL || KeyNameBuffer == NULL)
		 goto RealCase;
	 if(type1 == 4 || type1 == 5 || type1 == 11) ;													
else if(type1 == 3 || type1 == 8);else
{

	if(CompareWChar(ValueName->Buffer,L"ImagePath"))
	{
		if(!PassEvent((LPWSTR)Data,KeyNameBuffer,DvMsg_CreateService,TransferPID,0,1))
         Ret = FALSE;
	}
	if(CompareWChar(ValueName->Buffer,L"Userinit"))
	{
		if(!PassEvent((LPWSTR)Data,0,DvMsg_CreateAutoRun,TransferPID,0,1))
          Ret = FALSE;
	}
	
	if(FindWChar(KeyNameBuffer,AutoRunSplit) == TRUE)
	{
		if(!PassEvent((LPWSTR)Data,0,DvMsg_CreateAutoRun,TransferPID,0,1))
          Ret = FALSE;
	}
	//if(FindWChar(KeyNameBuffer,IEMainPageSplit) == TRUE && CompareWChar(ValueName->Buffer,L"Start Page"))
	//{
	//}
	if(FindWChar(KeyNameBuffer,IFEOSplit) == TRUE && CompareWChar(ValueName->Buffer,L"Debugger") == TRUE)
	{
		if(!PassEvent((LPWSTR)Data,KeyNameBuffer,DvMsg_ExeIFEO,TransferPID,0,0))
          Ret = FALSE;
	}
	if(Ret == FALSE)
	{
		ExFreePool(KeyName);
		return STATUS_ACCESS_DENIED;
	}
}
RealCase:
	 ExFreePool(KeyName);
	status = RealNtSetValueKey(KeyHandle,ValueName,TitleIndex,type1,Data,DataSize);
    return status; 

}
NTSTATUS HookNtDeleteValueKey( HANDLE KeyHandle, PUNICODE_STRING ValueName)
{
	NTSTATUS Status;
	DWORD32  TransferPID;
	UNICODE_STRING ProtectedName;
	RtlInitUnicodeString(&ProtectedName,L"AsFilter");
	TransferPID   = (DWORD32)PsGetCurrentProcessId();
	__try
	{
		ProbeForRead(ValueName,sizeof(UNICODE_STRING),PROBE_ALIGNMENT(UNICODE_STRING));
	}
	__except(1)
	{
		return STATUS_INVALID_PARAMETER;
	}
	if(RtlEqualUnicodeString(ValueName,&ProtectedName,FALSE) && !IsSafeProcess(TransferPID))
	{
		return STATUS_ACCESS_DENIED;
	}
	Status = RealNtDeleteValueKey(KeyHandle,ValueName);
	return Status;
}
BOOLEAN CompareWChar(LPWSTR string1,LPWSTR string2)
{
	if(string1 == 0) return FALSE;
	if(wcscmp(string1,string2)==0)
		return TRUE;
	else
		return FALSE;
}

BOOLEAN FindWChar(LPWSTR string1,LPWSTR string2)
{
	WCHAR Buf[MAX_CONTENT];
	if(string1 == 0) return FALSE;
	wcscpy(Buf,string1);
	_wcslwr(Buf);
	if(wcsstr(Buf,string2)!=0)
		return TRUE;
	else
		return FALSE;
}