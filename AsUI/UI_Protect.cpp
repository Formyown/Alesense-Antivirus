#include "UI_Protect.h"
#include "UI_Scan.h"
#include "LuaOperation.h"
static int ScanedCounts = 0;
static int FoundCounts = 0;
queue<PDRIVEREVENTMSG> PopWindowQueue;
HANDLE PopWindowEvent;
void OnProtectMessage(PVOID Message)
{
	PDRIVEREVENTMSG Event = (PDRIVEREVENTMSG)Message;
	PVIRUS_DESCRIPTION VD = (PVIRUS_DESCRIPTION)Event->SubContent;
	LPWSTR FilePath = Event->Content;
	if (Event->Sign != NULL)
	{
		if (Event->NeedScan) //不需要回调
			FoundCounts++;
		InformUI_Int(TypeOfInformUI::FoundCount, FoundCounts);
		ScanedCounts++;
		InformUI_Int(TypeOfInformUI::CreatedCount, ScanedCounts);
		InformUI_String(TypeOfInformUI::CreatePath, Event->Content);
		if (Event->Sign == 1)
			return;
	}
	PDRIVEREVENTMSG QueueMsg = (PDRIVEREVENTMSG)malloc(sizeof(DRIVEREVENTMSG));
	RtlCopyMemory(QueueMsg, Message, sizeof(DRIVEREVENTMSG));
	PopWindowQueue.push(QueueMsg);
}
void CreateProtectInterface()
{
	AsCreateNamedPipe(L"AsUIProtect", OnProtectMessage);
	PopWindowEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ProtectEventFilter, NULL, NULL, NULL);
}
void GetFileName(LPCWSTR Path, LPWSTR Name)
{
	WCHAR Buffer[MAX_PATH] = { 0 };
	_wsplitpath(Path, NULL, NULL, Name, Buffer);
	lstrcat(Name, Buffer);
	if (lstrlen(Name) >= 32)
	{
		Name[28] = '.';
		Name[29] = '.';
		Name[30] = '.';
		Name[31] = '\0';
	}
}
void GetShortPath(LPCWSTR Path, LPWSTR ShortPath)
{
	lstrcpy(ShortPath, Path);
	if (lstrlen(ShortPath) >= 44)
	{
		ShortPath[40] = '.';
		ShortPath[41] = '.';
		ShortPath[42] = '.';
		ShortPath[43] = '\0';
	}
}
void HandlePopWindow(PDRIVEREVENTMSG Msg, BOOL Result)
{

	SetEvent(PopWindowEvent);
	if (Msg == NULL)
		return;
	Msg->Result = Result;
	SendToCoreProtect(Msg);
	free(Msg);
}
void ProtectEventFilter()
{

	WCHAR ShortPath[2][32];
	WCHAR FText[4][64];
	PDRIVEREVENTMSG Event;
	while (1)
	{
		while (PopWindowQueue.empty())
			Sleep(50);
		Event = PopWindowQueue.front();
		PopWindowQueue.pop();
		if (Event->NeedScan)
		{
			InformUI_PopWindow(PopWarningWindow, 0, L"确定", NULL, TRUE, TRUE, L"检测到威胁对象", Event->Content, NULL, L"检测到威胁对象并已经阻止其运行");
		}
		else
			switch (Event->MessageType)
		{
			case DvMsg_CreateAutoRun:
				GetFileName(Event->TransferPath, ShortPath[0]);
				wsprintf(FText[1], L"所在进程:%s", ShortPath[0]);
				wsprintf(FText[2], L"启动项:%s", Event->Content);
				InformUI_PopWindow(PopWarningWindow, (DWORD)Event, L"阻止", L"允许", TRUE, TRUE, L"检测到进程创建自启动项", FText[1], FText[2], L"修改启动项是木马常用手段,如果您没有指定此次操作,建议禁止.");
				break;
			case DvMsg_CreateService:
				GetFileName(Event->TransferPath, ShortPath[0]);
				wsprintf(FText[1], L"所在进程:%s", ShortPath[0]);
				wsprintf(FText[2], L"服务:%s", Event->Content);
				InformUI_PopWindow(PopWarningWindow, (DWORD)Event, L"阻止", L"允许", TRUE, TRUE, L"检测到进程创建系统服务", FText[1], FText[2], L"创建系统服务是木马常用手段,如果您没有指定此次操作,建议禁止.");
				break;
			case DvMsg_ExeIFEO:   //注册表-映像劫持
				GetFileName(Event->TransferPath, ShortPath[0]);
				wsprintf(FText[1], L"可疑进程:%s", ShortPath[0]);
				wsprintf(FText[2], L"劫持目标:%s", Event->Content);

				InformUI_PopWindow(PopWarningWindow, (DWORD)Event, L"阻止", NULL, TRUE, TRUE, L"拦截到映像劫持行为", FText[1], FText[2], L"映像劫持为木马典型手段,正常软件极少使用,请阻止此操作");
				break;
			case DvMsg_LoadDriver:
				GetFileName(Event->TransferPath, ShortPath[0]);
				wsprintf(FText[1], L"所在进程:%s", ShortPath[0]);
				wsprintf(FText[2], L"驱动程序:%s", Event->Content);
				InformUI_PopWindow(PopWarningWindow, (DWORD)Event, L"阻止", L"允许", TRUE, TRUE, L"检测到进程加载驱动程序", FText[1], FText[2], L"加载驱动能使木马获取最高权限,如果您未在使用底层软件,建议禁止");
				break;
			default:

				break;
		}
		WaitForSingleObject(PopWindowEvent, INFINITE);
		ResetEvent(PopWindowEvent);
	}
}
