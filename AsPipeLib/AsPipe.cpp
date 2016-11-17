#include "AsPipe.h"

AsPipeClass::AsPipeClass(LPTSTR PipeName, ROUTINE_ONDATA OnDataRoutine)
{
	__try
	{
		lstrcpy(this->PipeName, L"\\\\.\\pipe\\");
		lstrcat(this->PipeName, PipeName);
		this->OnDataRoutine = OnDataRoutine;
		CreateThread(NULL, NULL, AsListenThread, this, 0, 0);
	}
	__except (1)
	{
		TerminateThread(this->ListenThreadHandle, 0);
		ErrorCode = Error;
	}
}
DWORD WINAPI AsListenThread(PVOID Arg)
{
	AsPipeClass* Pipe = (AsPipeClass*)Arg;
	DWORD Result, BytesRead;
	while (1)
	{
	Try1:
		Pipe->PipeHandle = CreateNamedPipe(Pipe->PipeName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 4096, 4096, 5000, NULL);
		if (Pipe->PipeHandle == INVALID_HANDLE_VALUE)
		{
			int Error = GetLastError();
			Sleep(100);
			goto Try1;
		}
	Try2:
		if (!ConnectNamedPipe(Pipe->PipeHandle, NULL))
		{
			if (GetLastError() != ERROR_PIPE_CONNECTED)
			{
				Sleep(100);
				goto Try2;
			}
		}
			Result = ReadFile(Pipe->PipeHandle, Pipe->InBuffer, 4096, &BytesRead, NULL);
			if (Result)
				Pipe->OnDataRoutine(Pipe->InBuffer);
		CloseHandle(Pipe->PipeHandle);
	}
}
UINT AsPipeClass::AsPipeGetLastError()
{
	return ErrorCode;
}
AsPipeClass::~AsPipeClass()
{
	DisconnectNamedPipe(this->PipeHandle);
	CloseHandle(this->PipeHandle);
	TerminateThread(this->ListenThreadHandle, 0);
	delete this;
}
/*
FuncName AsCreateNamedPipe
	Create a New NamedPipe
Argument:
	PipeName a pointer to the Name of the pipe
	OnDataRoutine the callback routine typed as the ROUTINE_ONDATA (see ROUTINE_ONDATA in namespace AsPipe)
Return value:
	The Pointer of the Class
	if the PipeName is invalid it will return 0
*/
PVOID AsCreateNamedPipe(LPTSTR PipeName, ROUTINE_ONDATA OnDataRoutine)
{
	AsPipeClass* Pipe = new AsPipeClass(PipeName, OnDataRoutine);
	
	if (Pipe->AsPipeGetLastError())
		return 0;
	else
		return Pipe;
}
BOOL AsSendToNamedPipe(LPTSTR PipeName, PVOID Data,UINT SizeOfData)
{
	DWORD SizeOfDataWritten;
	HANDLE Handle;
	DWORD Mode = PIPE_READMODE_MESSAGE;
	WCHAR PipeFullName[32] = { 0 };
	BOOL Result;
	INT Try = 20;
	lstrcpy(PipeFullName, L"\\\\.\\pipe\\");
	lstrcat(PipeFullName, PipeName);
	while (Try--)
	{
		if (WaitNamedPipe(PipeFullName, 5000))
			goto Nxt;
		Sleep(50);
	}
	return FALSE;
	Nxt:
	Handle = CreateFile(PipeFullName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	if (Handle == INVALID_HANDLE_VALUE)
		return FALSE;
	Result = SetNamedPipeHandleState(Handle, &Mode, 0, 0);

	if (!Result)
		return FALSE;
	WriteFile(Handle, Data, SizeOfData, &SizeOfDataWritten, 0);
	CloseHandle(Handle);
	if (SizeOfDataWritten != SizeOfData)
		return FALSE;
	return TRUE;
}
BOOL AsCloseNamedPipe(PVOID Pipe)
{
	AsPipeClass* ThePipe = (AsPipeClass*)Pipe;
	__try
	{
		ThePipe->~AsPipeClass(); 
	}
	__except (1)
	{
		return FALSE;
	}
	return TRUE;
}