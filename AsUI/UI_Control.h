#include <Windows.h>
#include "..\Libs\AsPipeLib.h"

#define SendToCoreControl(Data) AsSendToNamedPipe(L"AsCoreControl",(PVOID)Data,sizeof(NTCONTROLMSG))
void ForceDeleteFile(LPWSTR FilePath);
typedef enum NtControlCode
{
	ForceDelete
};
typedef struct
{
	DWORD32 ControlCode;
	WCHAR ControlMsg[512];
}NTCONTROLMSG, *PNTCONTROLMSG;
void OnCoreControl(PVOID Data);