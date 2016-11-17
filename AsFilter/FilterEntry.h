#pragma once
#include <ntifs.h>
NTSTATUS DriverEntry( IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING theRegistryPath);
void DriverUnload( IN PDRIVER_OBJECT DriverObject);
NTSTATUS OnMaJor(PDEVICE_OBJECT device,PIRP irp);