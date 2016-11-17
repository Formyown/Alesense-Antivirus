#include "AsNtControl.h"

BOOL InitializeNtControl()
{
	DriverHandle = CreateFile(DRIVER_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (DriverHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(0, L"驱动程序连接失败 请运行目录下的InstallDriver.exe来安装驱动程序", L"", MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}
BOOL TerminateNtControl()
{
	return CloseHandle(DriverHandle);
}
BOOL ForceDeleteFile(LPWSTR FilePath)
{
	BOOL Result = 0;
	DWORD Length;
	WCHAR Buffer[MAX_PATH] = { 0 };
	lstrcpy(Buffer, FilePath);
	DeviceIoControl(DriverHandle, CTL_FORCEDELETE, Buffer, MAX_PATH*2, &Result, sizeof(BOOL), &Length, 0);
	if (Length == sizeof(BOOL))
		return Result;
	return FALSE;
}
BOOL GetPathByPID(HANDLE ProcessId,LPWSTR Buffer)
{
	BOOL Result;
	DWORD Length;
	Result = DeviceIoControl(DriverHandle, CTL_GETPATHBYPID, &ProcessId, sizeof(HANDLE), Buffer, 512*2, &Length, 0);
	if (Result == 0x00000000L)//SUCCESS
		return TRUE;
	return FALSE;
}

