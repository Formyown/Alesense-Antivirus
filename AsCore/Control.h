#include <Windows.h>
#include "..\Libs\aspipelib.h"
#pragma comment(lib,"..\\Libs\\AsNtControl.lib")
#pragma comment(lib,"..\\Libs\\AsPipeLib.lib")
#pragma once
extern "C"
{
	extern _declspec(dllimport) BOOL ForceDeleteFile(LPWSTR FilePath);
	extern _declspec(dllimport)	BOOL GetPathByPID(HANDLE ProcessId, LPWSTR Buffer);
	extern _declspec(dllimport) BOOL InitializeNtControl();
	extern _declspec(dllimport) BOOL TerminateNtControl();
}
class CControl
{
private:
	PVOID Pipe;
public:
	CControl();
	~CControl();
};
typedef enum NtControlCode
{
	ForceDelete
};
typedef struct
{
	DWORD32 ControlCode;
	WCHAR ControlMsg[512];
}NTCONTROLMSG,*PNTCONTROLMSG;
void OnCoreControl(PVOID Data);