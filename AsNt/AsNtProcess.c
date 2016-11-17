#include "AsNt.h"
typedef NTSTATUS(*ZWQUERYINFORMATIONPROCESS)(HANDLE ProcessHandle, PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);
ZWQUERYINFORMATIONPROCESS ZwQueryInformationProcess = NULL;
NTSTATUS ImagePathConvertToRealPath(LPCWSTR FileName, LPCWSTR OutFileName)
{
	HANDLE	FileHandle;
	OBJECT_ATTRIBUTES ObjectAttribute;
	IO_STATUS_BLOCK IoStackBlock;
	UNICODE_STRING uFileName;
	NTSTATUS	Status;
	PFILE_OBJECT FileObject;
	RtlInitUnicodeString(&uFileName, FileName);
	InitializeObjectAttributes(&ObjectAttribute, &uFileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 0, 0);
	Status = IoCreateFile(&FileHandle,FILE_READ_ATTRIBUTES,&ObjectAttribute,&IoStackBlock,0,FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,FILE_OPEN,FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,0,0,CreateFileTypeNone,0,IO_NO_PARAMETER_CHECKING);
	if (!NT_SUCCESS(Status))
	{
		return Status;
	}
	Status = ObReferenceObjectByHandle(FileHandle,GENERIC_READ,*IoFileObjectType,KernelMode,&FileObject,NULL);
	if (!NT_SUCCESS(Status) || !FileObject ||!FileObject->FileName.Length ||!FileObject->FileName.Buffer)
	{
		ZwClose(FileHandle);
		return STATUS_INVALID_PARAMETER;
	}
{
	UNICODE_STRING VolumeName;
	UNICODE_STRING FinalName;
	UINT32	WillFreeVolumeName = TRUE;
	VolumeName.Buffer = NULL;
	VolumeName.Length = 0;
	VolumeName.MaximumLength = 0;
	Status = RtlVolumeDeviceToDosName(FileObject->DeviceObject, &VolumeName);
	if (!NT_SUCCESS(Status))
	{
		RtlInitUnicodeString(&VolumeName, L"\\");
		WillFreeVolumeName = FALSE;
	}
	if (FileObject->FileName.Length + VolumeName.Length >= 1024)
	{
		ObDereferenceObject(FileObject);
		ZwClose(FileHandle);
		if (WillFreeVolumeName && MmIsAddressValid(VolumeName.Buffer))
		{
			ExFreePool(VolumeName.Buffer);
		}
		return STATUS_INVALID_PARAMETER;
	}

	RtlZeroMemory((PVOID)OutFileName, 1024);
	RtlInitUnicodeString(&FinalName, OutFileName);
	FinalName.MaximumLength = 1024;
	if (!NT_SUCCESS(RtlAppendUnicodeStringToString(&FinalName, &VolumeName)))
	{
		ObDereferenceObject(FileObject);
		ZwClose(FileHandle);
		if (WillFreeVolumeName && MmIsAddressValid(VolumeName.Buffer))
		{
			ExFreePool(VolumeName.Buffer);
		}
		return STATUS_INVALID_PARAMETER;
	}

	if (!NT_SUCCESS(RtlAppendUnicodeStringToString(&FinalName, &FileObject->FileName)))
	{
		ObDereferenceObject(FileObject);
		ZwClose(FileHandle);
		if (WillFreeVolumeName && MmIsAddressValid(VolumeName.Buffer))
		{
			ExFreePool(VolumeName.Buffer);
		}
		return STATUS_INVALID_PARAMETER;
	}
	ObDereferenceObject(FileObject);
	ZwClose(FileHandle);
	if (WillFreeVolumeName && MmIsAddressValid(VolumeName.Buffer))
	{
		ExFreePool(VolumeName.Buffer);
	}
	return STATUS_SUCCESS;
}
}
NTSTATUS GetProcessImagePath(IN  ULONG   dwProcessId, OUT PUNICODE_STRING ProcessImagePath)
{
	NTSTATUS Status;
	HANDLE hProcess;
	PEPROCESS pEprocess;
	ULONG returnedLength;
	ULONG bufferLength;
	PVOID buffer;
	PUNICODE_STRING imageName;
	PAGED_CODE();
	if (NULL == ZwQueryInformationProcess)
	{
		UNICODE_STRING routineName;
		RtlInitUnicodeString(&routineName, L"ZwQueryInformationProcess");
		ZwQueryInformationProcess = (ZWQUERYINFORMATIONPROCESS)MmGetSystemRoutineAddress(&routineName);
		if (NULL == ZwQueryInformationProcess)
		{
			return STATUS_INSUFFICIENT_RESOURCES;
		}
	}
	Status = PsLookupProcessByProcessId((HANDLE)dwProcessId, &pEprocess);
	if (!NT_SUCCESS(Status))
		return Status;
	Status = ObOpenObjectByPointer(pEprocess, OBJ_KERNEL_HANDLE, NULL, GENERIC_READ, *PsProcessType, KernelMode, &hProcess);
	if (!NT_SUCCESS(Status))
		return Status;
	Status = ZwQueryInformationProcess(hProcess, ProcessImageFileName, NULL, 0, &returnedLength);
	if (STATUS_INFO_LENGTH_MISMATCH != Status)
	{
		return Status;
	}
	bufferLength = returnedLength - sizeof(UNICODE_STRING);
	if (ProcessImagePath->MaximumLength < bufferLength)
	{
		ProcessImagePath->Length = (USHORT)bufferLength;
		return STATUS_BUFFER_OVERFLOW;
	}
	buffer = ExAllocatePoolWithTag(PagedPool, returnedLength, 'AsN');
	if (NULL == buffer)
		return STATUS_INSUFFICIENT_RESOURCES;
	Status = ZwQueryInformationProcess(hProcess, ProcessImageFileName, buffer, returnedLength, &returnedLength);
	if (NT_SUCCESS(Status))
	{
		RtlCopyUnicodeString(ProcessImagePath, (PUNICODE_STRING)buffer);
		Status = ImagePathConvertToRealPath(ProcessImagePath->Buffer, ProcessImagePath->Buffer);
	}
	ZwClose(hProcess);
	ExFreePoolWithTag(buffer, 'AsN');
	return Status;

}
