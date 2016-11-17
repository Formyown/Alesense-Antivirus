#include "FilterEntry.h"
#include "FilterCommunication.h"
#include "FilterFrame.h"
#include <wdmsec.h>
#pragma comment(lib, "bufferoverflowK.lib") 
UNICODE_STRING symbol_link;
UNICODE_STRING device_name;
PDEVICE_OBJECT device;
NTSTATUS DriverEntry( IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING theRegistryPath )
{
	NTSTATUS status;
	int i = 0;
    UNICODE_STRING device_name = RTL_CONSTANT_STRING(L"\\Device\\AsFilterService");
	status = IoCreateDevice(DriverObject,sizeof(DRIVEREVENTMSG),&device_name,FILE_DEVICE_UNKNOWN,FILE_DEVICE_SECURE_OPEN,FALSE,&device);
	for (i = 0;i < IRP_MJ_MAXIMUM_FUNCTION;i++)
	DriverObject->MajorFunction[i] = OnMaJor;   
	if(!NT_SUCCESS(status))
	{
		DbgPrint("Failed\n");
		return status;
	}
	if(IoIsWdmVersionAvailable(1,0x10))
	{
		RtlInitUnicodeString(&symbol_link,L"\\DosDevices\\Global\\AsFilterSymbol");
	}
	else
	{
		RtlInitUnicodeString(&symbol_link,L"\\DosDevices\\AsFilterSymbol");
	}
	IoCreateSymbolicLink(&symbol_link,&device_name);

	DriverObject->DriverUnload  = DriverUnload;						//DriverUnLoad Àý³Ì
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControl; //IoControl Àý³Ì×¢²á
	LoadFrame();
	return STATUS_SUCCESS;
}
void DriverUnload( IN PDRIVER_OBJECT DriverObject)
{
	UnLoadFrame();
	IoDeleteDevice(DriverObject->DeviceObject);
}
NTSTATUS OnMaJor(PDEVICE_OBJECT device,PIRP irp)
{
	irp->IoStatus.Information = 0;
	irp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(irp,IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}