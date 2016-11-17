#include "FilterRoutine.h"
#include "FilterCommunication.h"
#include "FilterFrame.h"
NTSTATUS HookNtLoadDriver(IN PUNICODE_STRING DriverServiceName)
{
	DWORD32 TransferPID;
	DWORD32 Status;
	TransferPID = (DWORD32)PsGetCurrentProcessId();
	__try
	{
		ProbeForRead(DriverServiceName,sizeof(UNICODE_STRING),PROBE_ALIGNMENT(UNICODE_STRING));
	}
	__except(1)
	{
		return STATUS_INVALID_PARAMETER;
	}
	if(DriverServiceName->Buffer == 0)
		return RealNtLoadDriver(DriverServiceName);
	DbgPrint("[NtLoadDriver]%ws\n",DriverServiceName->Buffer);
	if(PassEvent(DriverServiceName->Buffer,0,DvMsg_LoadDriver,TransferPID,0,TRUE))
		return RealNtLoadDriver(DriverServiceName);
	else
		return STATUS_ACCESS_DENIED;
}


NTSTATUS HookNtSetSystemInformation( ULONG ControlNumber,  PVOID Information,  ULONG SizeofInformation)
{
	#define SystemCallAndLoadImage 38 //调用加载驱动
	//==============================
	if(ControlNumber == SystemCallAndLoadImage)
	{
			NTSTATUS Status;
			DWORD32 TransferPID = (DWORD32)PsGetCurrentProcessId();
			PSYSTEM_LOAD_AND_CALL_IMAGE DriverImage = (PSYSTEM_LOAD_AND_CALL_IMAGE)Information;
			__try
			{
				ProbeForRead(DriverImage,sizeof(SYSTEM_LOAD_AND_CALL_IMAGE),PROBE_ALIGNMENT(SYSTEM_LOAD_AND_CALL_IMAGE));
				ProbeForRead(&DriverImage->ModuleName,sizeof(UNICODE_STRING),PROBE_ALIGNMENT(UNICODE_STRING));
			}
			__except(1)
			{
				return STATUS_INVALID_PARAMETER;
			}
			DbgPrint("[NtSetSystemInformation]%ws\n",DriverImage->ModuleName.Buffer);
			if(PassEvent(DriverImage->ModuleName.Buffer,0,DvMsg_LoadDriver,TransferPID,1,TRUE))
				return RealNtSetSystemInformation(ControlNumber,Information,SizeofInformation);
			else
				return STATUS_ACCESS_DENIED;
	}
	return RealNtSetSystemInformation(ControlNumber,Information,SizeofInformation);
}