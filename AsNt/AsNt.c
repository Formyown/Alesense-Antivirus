#include "AsNt.h"
#pragma comment(lib, "bufferoverflowK.lib") 
UNICODE_STRING symbol_link;
UNICODE_STRING device_name;
PDEVICE_OBJECT device;
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING theRegistryPath)
{
	NTSTATUS status;
	int i = 0;
	UNICODE_STRING device_name = RTL_CONSTANT_STRING(L"\\Device\\AsNt");
	status = IoCreateDevice(DriverObject, NULL, &device_name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &device);
	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
		DriverObject->MajorFunction[i] = OnMaJor;
	if (!NT_SUCCESS(status))
	{
		DbgPrint("Failed\n");
		return status;
	}
	if (IoIsWdmVersionAvailable(1, 0x10))
	{
		RtlInitUnicodeString(&symbol_link, L"\\DosDevices\\Global\\AsNtSymbol");
	}
	else
	{
		RtlInitUnicodeString(&symbol_link, L"\\DosDevices\\AsNtSymbol");
	}
	IoCreateSymbolicLink(&symbol_link, &device_name);

	DriverObject->DriverUnload = DriverUnload;						//DriverUnLoad Àý³Ì
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControl; //IoControl Àý³Ì×¢²á
	return STATUS_SUCCESS;
}
void DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
	IoDeleteDevice(DriverObject->DeviceObject);
}
NTSTATUS OnMaJor(PDEVICE_OBJECT device, PIRP irp)
{
	irp->IoStatus.Information = 0;
	irp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}
NTSTATUS IoControl(PDEVICE_OBJECT DriverObject, PIRP Irp)
{
	PIO_STACK_LOCATION Irpsp = IoGetCurrentIrpStackLocation(Irp);
	DWORD32 Code = Irpsp->Parameters.DeviceIoControl.IoControlCode;
	DWORD32 In_Size = Irpsp->Parameters.DeviceIoControl.InputBufferLength;
	DWORD32 Out_Size = Irpsp->Parameters.DeviceIoControl.OutputBufferLength;
	PVOID Buffer = Irp->AssociatedIrp.SystemBuffer;
	NTSTATUS Status;
	INT Res;
	Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
	Irp->IoStatus.Information = 0;

	//===========================================//

	if (Code == CTL_FORCEDELETE)
	{
		if (MmIsAddressValid(Buffer))
		{
			Res = ForceDeleteFile(Buffer);
			RtlMoveMemory(Buffer, &Res, sizeof(INT32));
			Irp->IoStatus.Status = STATUS_SUCCESS;
			Irp->IoStatus.Information = sizeof(INT32);
		}
		
	}
	if (Code == CTL_GETPATHBYPID)
	{
		if (MmIsAddressValid(Buffer))
		{
			UNICODE_STRING Uni = {0};
			Uni.MaximumLength = 1024;
			Uni.Buffer = Buffer;
			Res = GetProcessImagePath(*(PULONG)Buffer, &Uni);
			Irp->IoStatus.Status = Res;
			Irp->IoStatus.Information = 1024;
		}
	}
	//============================================//
Exit:
	//============================================//	
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Irp->IoStatus.Status;
}
