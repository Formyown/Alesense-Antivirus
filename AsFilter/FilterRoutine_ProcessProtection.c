#include "FilterRoutine.h"
#include "FilterCommunication.h"
#include "FilterFrame.h"
NTSTATUS HookNtCreateUserProcess(OUT PHANDLE ProcessHandle,OUT PHANDLE ThreadHandle, ACCESS_MASK ProcessDesiredAccess, ACCESS_MASK ThreadDesiredAccess, POBJECT_ATTRIBUTES ProcessObjectAttributes OPTIONAL, POBJECT_ATTRIBUTES ThreadObjectAttributes OPTIONAL, DWORD32 CreateProcessFlags, DWORD32 CreateThreadFlags, PRTL_USER_PROCESS_PARAMETERS ProcessParameters, PVOID Parameter9, PNT_PROC_THREAD_ATTRIBUTE_LIST AttributeList)
{
	PWCHAR FileName = 0;
	DWORD32 TransferPID;
	NTSTATUS status;
	TransferPID = (DWORD32)PsGetCurrentProcessId();
	__try
	{

		ProbeForRead(&ProcessParameters->ImagePathName, sizeof(UNICODE_STRING), PROBE_ALIGNMENT(UNICODE_STRING));
		ProbeForRead(ProcessParameters->ImagePathName.Buffer, ProcessParameters->ImagePathName.Length, sizeof(WCHAR));
		FileName = ProcessParameters->ImagePathName.Buffer;
	}
	__except (1)
	{
		__try
		{
			ProbeForRead(&ProcessParameters->CommandLine, sizeof(UNICODE_STRING), PROBE_ALIGNMENT(UNICODE_STRING));
			ProbeForRead(ProcessParameters->CommandLine.Buffer, ProcessParameters->CommandLine.Length, PROBE_ALIGNMENT(WCHAR));
			FileName = ProcessParameters->CommandLine.Buffer;
		}
		__except (1)
		{
		return RealNtCreateUserProcess(ProcessHandle,ThreadHandle,ProcessDesiredAccess,ThreadDesiredAccess,ProcessObjectAttributes,ThreadObjectAttributes,CreateProcessFlags,CreateThreadFlags,ProcessParameters,Parameter9,AttributeList);
		}
	}
	if(PassEvent(FileName,0,DvMsg_File,TransferPID,0,TRUE))
		return RealNtCreateUserProcess(ProcessHandle,ThreadHandle,ProcessDesiredAccess,ThreadDesiredAccess,ProcessObjectAttributes,ThreadObjectAttributes,CreateProcessFlags,CreateThreadFlags,ProcessParameters,Parameter9,AttributeList);
	else
		return STATUS_ACCESS_DENIED;
	return 0;
}

NTSTATUS HookNtCreateSection ( PHANDLE SectionHandle, ACCESS_MASK DesiredAccess,  POBJECT_ATTRIBUTES ObjectAttributes, PLARGE_INTEGER MaximumSize, DWORD32 SectionPageProtection, DWORD32 AllocationAttributes, HANDLE FileHandle)
{
PFILE_OBJECT    FileObject;
POBJECT_NAME_INFORMATION FilePath;
DWORD32 TargetPID;
DWORD32 TransferPID;
NTSTATUS Status;
if (SectionPageProtection & (PAGE_EXECUTE|PAGE_EXECUTE_READ|PAGE_EXECUTE_READWRITE|PAGE_EXECUTE_WRITECOPY) )
{
	TransferPID = (DWORD32)PsGetCurrentProcessId();
	if(NT_SUCCESS(ObReferenceObjectByHandle(FileHandle,0,NULL,KernelMode,(PVOID*)&FileObject,NULL)))
	{
		Status = IoQueryFileDosDeviceName(FileObject,&FilePath);
		if (NT_SUCCESS(Status))
		{
			DbgPrint("[NtCreateSection]%ws\n",&FileObject->FileName);
			ExFreePool(FilePath);
			ObDereferenceObject(FileObject);
		}
		else
		ObDereferenceObject(FileObject);
	}
}
	Status = RealNtCreateSection(SectionHandle,DesiredAccess,ObjectAttributes,MaximumSize,SectionPageProtection,AllocationAttributes,FileHandle);    
	return Status;
}




NTSTATUS HookNtCreateProcessEx(PHANDLE ProcessHandle,ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes,HANDLE InheritFromProcessHandle,HANDLE InheritHandles,HANDLE SectionHandle,HANDLE DebugPort,HANDLE ExceptionPort,HANDLE Unknown)
{
DWORD32 ProcessID;
DWORD32 Result = FALSE;
NTSTATUS   Status;   
PVOID   Section = NULL;
PVOID File = NULL;   
WCHAR RealPathBuffer[512] = {0};
UNICODE_STRING RealPath;   
UNICODE_STRING   DosPath;  
ProcessID=(DWORD32)PsGetCurrentProcessId();
RealPath.Buffer   = RealPathBuffer;
RealPath.MaximumLength   =   512*2;  
RealPath.Length=0;    
Status = ObReferenceObjectByHandle(SectionHandle,0,NULL,KernelMode,&Section,NULL);   
if(!NT_SUCCESS(Status)) goto Orig;  
__try 
{  

   File=(PVOID)*(ULONG*)((char*)Section+20);   
   File=(PVOID)*(ULONG*)((char*)File);   
   File=(PVOID)*(ULONG*)((char*)File+36);  
   ObReferenceObjectByPointer(File,0,NULL,KernelMode);   
   RtlVolumeDeviceToDosName(((PFILE_OBJECT)File)->DeviceObject,&DosPath);   
   RtlCopyUnicodeString(&RealPath,&DosPath);   
   RtlAppendUnicodeStringToString(&RealPath,&((PFILE_OBJECT)File)->FileName); 
   ObDereferenceObject(File);
   ObDereferenceObject(Section);
   ExFreePool(DosPath.Buffer);
   Result = PassEvent(RealPathBuffer,0,DvMsg_File,ProcessID,0,TRUE);
	if(Result == FALSE) 
		return STATUS_ACCESS_DENIED;
}  
__except(1)
{   
   if(Section) ObDereferenceObject(Section);
   goto Orig;
}  
Orig:
	return RealNtCreateProcessEx(ProcessHandle,DesiredAccess,ObjectAttributes,InheritFromProcessHandle,InheritHandles,SectionHandle,DebugPort,ExceptionPort,Unknown);
}
NTSTATUS HookNtUserMessageCall(PVOID Handle, INT Message, INT P1, INT P2)
{
	DWORD32 PID = PsGetCurrentProcessId();

		
		if (Handle == 0xffffffff && Message == 0x0016) //boardcase
			if (PassEvent(0, 0, DvMsg_Shutdown,PID,0,1 ))
			IsOfflineMode = TRUE;

	return RealNtUserMessageCall(Handle, Message, P1, P2);
}











//===========
BOOLEAN RegFilter(PUNICODE_STRING c1,PUNICODE_STRING c2,PWCHAR ld)
{
	__try
	{
	RtlInitUnicodeString(c1,ld);
	if(RtlEqualUnicodeString(c1,c2, TRUE) == TRUE)
	{
		return TRUE;
	}
	}
	__except(1)
	{
		return FALSE;
	}
	return FALSE;
}
BOOLEAN FindReg(PUNICODE_STRING src,PWCHAR sub)
{
		__try
		{
			
			if(wcsstr((LPWSTR)src,sub)!=NULL)
			{
			return TRUE;
			}
		}
		__except(1)
		{
			return FALSE;
		}
	
	return FALSE;
}
/*=======================================================================================*/