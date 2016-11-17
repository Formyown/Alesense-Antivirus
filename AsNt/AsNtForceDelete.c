#include "AsNt.h"
NTSTATUS FD_SetFileCompletion(IN PDEVICE_OBJECT DeviceObject,IN PIRP Irp,IN PVOID Context)
{
	Irp->UserIosb->Status = Irp->IoStatus.Status;
	Irp->UserIosb->Information = Irp->IoStatus.Information;

	KeSetEvent(Irp->UserEvent, IO_NO_INCREMENT, FALSE);

	IoFreeIrp(Irp);
	return STATUS_MORE_PROCESSING_REQUIRED;
}

HANDLE  FD_OpenFile(WCHAR *szFileName)
{
	NTSTATUS            ntStatus;
	UNICODE_STRING      FileName;
	OBJECT_ATTRIBUTES   objectAttributes;
	HANDLE              hFile;
	IO_STATUS_BLOCK     ioStatus;
	// 确保IRQL在PASSIVE_LEVEL上   
	if (KeGetCurrentIrql() > PASSIVE_LEVEL)
		return NULL;

	// 初始化文件名   
	RtlInitUnicodeString(&FileName, szFileName);
	DbgPrint("%ws", FileName.Buffer);

	//初始化对象属性   
	InitializeObjectAttributes(&objectAttributes, &FileName, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);

	// 打开文件   
	ntStatus = IoCreateFile(&hFile, FILE_READ_ATTRIBUTES, &objectAttributes, &ioStatus, 0, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_DELETE, FILE_OPEN, 0, NULL, 0, CreateFileTypeNone, NULL, IO_NO_PARAMETER_CHECKING);
	if (!NT_SUCCESS(ntStatus))
		return NULL;

	return  hFile;
}

BOOLEAN FD_StripFileAttributes(HANDLE FileHandle)
{
	NTSTATUS                ntStatus = STATUS_SUCCESS;
	PFILE_OBJECT            fileObject;
	PDEVICE_OBJECT          DeviceObject;
	PIRP                    Irp;
	KEVENT                  SycEvent;
	FILE_BASIC_INFORMATION  FileInformation;
	IO_STATUS_BLOCK         ioStatus;
	PIO_STACK_LOCATION      irpSp;
	ntStatus = ObReferenceObjectByHandle(FileHandle, DELETE, *IoFileObjectType, KernelMode, (PVOID*)&fileObject, NULL);
	if (!NT_SUCCESS(ntStatus))
		return FALSE;
	DeviceObject = IoGetRelatedDeviceObject(fileObject);
	Irp = IoAllocateIrp(DeviceObject->StackSize, TRUE);
	if (Irp == NULL)
	{
		ObDereferenceObject(fileObject);
		return FALSE;
	}
	KeInitializeEvent(&SycEvent, SynchronizationEvent, FALSE);
	memset(&FileInformation, 0, 0x28);
	FileInformation.FileAttributes = FILE_ATTRIBUTE_NORMAL;
	Irp->AssociatedIrp.SystemBuffer = &FileInformation;
	Irp->UserEvent = &SycEvent;
	Irp->UserIosb = &ioStatus;
	Irp->Tail.Overlay.OriginalFileObject = fileObject;
	Irp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	Irp->RequestorMode = KernelMode;
	irpSp = IoGetNextIrpStackLocation(Irp);
	irpSp->MajorFunction = IRP_MJ_SET_INFORMATION;
	irpSp->DeviceObject = DeviceObject;
	irpSp->FileObject = fileObject;
	irpSp->Parameters.SetFile.Length = sizeof(FILE_BASIC_INFORMATION);
	irpSp->Parameters.SetFile.FileInformationClass = FileBasicInformation;
	irpSp->Parameters.SetFile.FileObject = fileObject;
	IoSetCompletionRoutine(Irp, FD_SetFileCompletion, NULL, TRUE, TRUE, TRUE);
	IoCallDriver(DeviceObject, Irp);
	KeWaitForSingleObject(&SycEvent, Executive, KernelMode, TRUE, NULL);
	ObDereferenceObject(fileObject);
	return TRUE;
}

INT32 FD_DeleteFile(HANDLE FileHandle)
{
	NTSTATUS          ntStatus = STATUS_SUCCESS;
	PFILE_OBJECT      fileObject;
	PDEVICE_OBJECT    DeviceObject;
	PIRP              Irp;
	KEVENT            SycEvent;
	FILE_DISPOSITION_INFORMATION    FileInformation;
	IO_STATUS_BLOCK                 ioStatus;
	PIO_STACK_LOCATION              irpSp;
	PSECTION_OBJECT_POINTERS        pSectionObjectPointer;
	ntStatus = ObReferenceObjectByHandle(FileHandle, DELETE, *IoFileObjectType, KernelMode, (PVOID*)&fileObject, NULL);
	if (!NT_SUCCESS(ntStatus))
		return FALSE;
	DeviceObject = IoGetRelatedDeviceObject(fileObject);
	Irp = IoAllocateIrp(DeviceObject->StackSize, TRUE);
	if (Irp == NULL)
	{
		ObDereferenceObject(fileObject);
		return FALSE;
	}
	KeInitializeEvent(&SycEvent, SynchronizationEvent, FALSE);
	FileInformation.DeleteFile = TRUE;
	Irp->AssociatedIrp.SystemBuffer = &FileInformation;
	Irp->UserEvent = &SycEvent;
	Irp->UserIosb = &ioStatus;
	Irp->Tail.Overlay.OriginalFileObject = fileObject;
	Irp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	Irp->RequestorMode = KernelMode;
	irpSp = IoGetNextIrpStackLocation(Irp);
	irpSp->MajorFunction = IRP_MJ_SET_INFORMATION;
	irpSp->DeviceObject = DeviceObject;
	irpSp->FileObject = fileObject;
	irpSp->Parameters.SetFile.Length = sizeof(FILE_DISPOSITION_INFORMATION);
	irpSp->Parameters.SetFile.FileInformationClass = FileDispositionInformation;
	irpSp->Parameters.SetFile.FileObject = fileObject;
	IoSetCompletionRoutine(Irp, FD_SetFileCompletion, NULL, TRUE, TRUE, TRUE);
	// 设置属性让文件可以删除
	pSectionObjectPointer = fileObject->SectionObjectPointer;
	pSectionObjectPointer->ImageSectionObject = 0;
	pSectionObjectPointer->DataSectionObject = 0;
	IoCallDriver(DeviceObject, Irp);
	KeWaitForSingleObject(&SycEvent, Executive, KernelMode, TRUE, NULL);
	ObDereferenceObject(fileObject);
	return TRUE;
}

BOOLEAN ForceDeleteFile(WCHAR *szFileName)
{
	HANDLE      hFile = NULL;
	INT32     status = FALSE;
	WCHAR FilePath[512] = {0};

	__try
	{
		wcscpy(FilePath, L"\\DosDevices\\");
		wcscat(FilePath, szFileName);
		if ((hFile = FD_OpenFile(FilePath)) == NULL)
			return FALSE;

		if (FD_StripFileAttributes(hFile) == FALSE)
		{
			ZwClose(hFile);
			return FALSE;
		}
		status = FD_DeleteFile(hFile);
		ZwClose(hFile);
		return status;

	}
	__except (1)
	{
		return FALSE;
	}
}
