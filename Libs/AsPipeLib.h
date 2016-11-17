#include <Windows.h>
#pragma comment (lib,"..\\Libs\\AsPipeLib.lib")
#pragma once

typedef void(*ROUTINE_ONDATA)(PVOID Data);
PVOID AsCreateNamedPipe(LPTSTR PipeName, ROUTINE_ONDATA OnDataRoutine);
BOOL AsSendToNamedPipe(LPTSTR PipeName, PVOID Data, UINT SizeOfData);
BOOL AsCloseNamedPipe(PVOID Pipe);
class AsPipeClass
{
public:
	typedef enum AsPipe_ErrorCode
	{
		NoError,
		Error
	};
public:
	AsPipeClass(LPTSTR PipeName, ROUTINE_ONDATA OnDataRoutine);
	~AsPipeClass();
	UINT AsPipeGetLastError();
	BYTE InBuffer[4096]; //Buffer to receive the data
	TCHAR PipeName[32];  //The Name of the Pipe
	HANDLE PipeHandle;
	ROUTINE_ONDATA OnDataRoutine = 0;
private:
	HANDLE ListenThreadHandle = 0;
	AsPipe_ErrorCode ErrorCode = NoError;
	friend DWORD WINAPI AsListenThread(PVOID Arg);
};

