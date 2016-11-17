#include <Windows.h>

#define CTL_FORCEDELETE (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN,0xDD0,METHOD_BUFFERED,FILE_ALL_ACCESS)
#define CTL_GETPATHBYPID (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN,0xDD1,METHOD_BUFFERED,FILE_ALL_ACCESS)
#define DRIVER_NAME L"\\\\.\\AsNtSymbol"
extern "C"
{
	extern _declspec(dllexport) BOOL ForceDeleteFile(LPWSTR FilePath);
	extern _declspec(dllexport)	BOOL GetPathByPID(HANDLE ProcessId, LPWSTR Buffer);
	extern _declspec(dllexport) BOOL InitializeNtControl();
	extern _declspec(dllexport) BOOL TerminateNtControl();
}
static HANDLE DriverHandle;