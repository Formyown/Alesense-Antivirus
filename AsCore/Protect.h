#pragma once
#include <windows.h>
#include "Identify.h"
#include "Core.h"
#include "..\Libs\AsPipeLib.h"

void ScanCallBack(LPWSTR FilePath,PVIRUS_DESCRIPTION VD,VirusType Danger,DWORD32 SelfPtr,DWORD32 Reserved2);
void UICallBack(PVOID Message);
static CIdentify* IdentifyMod;
static HANDLE FilterDriverHandle; //驱动的句柄
class CProtect
{
#define SendToUIProtect(Data,Size) AsSendToNamedPipe(L"AsUIProtect",(PVOID)Data,Size)
#define CTL_WAITFOREVENT (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN,0xAF0,METHOD_BUFFERED,FILE_ALL_ACCESS)
#define CTL_STARTPROTECTION (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN,0xAF1,METHOD_BUFFERED,FILE_ALL_ACCESS)
#define CTL_STOPPROTECTION (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN,0xAF2,METHOD_BUFFERED,FILE_ALL_ACCESS)
#define CTL_EVENTCALLBACK (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN,0xAF3,METHOD_BUFFERED,FILE_ALL_ACCESS)
#define CTL_INSERTPROTECTPID (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN,0xAF4,METHOD_BUFFERED,FILE_ALL_ACCESS)
#define CTL_REMOVEPROTECTPID (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN,0xAF5,METHOD_BUFFERED,FILE_ALL_ACCESS)
#define BeginThread(Routine,Argument) CreateThread(0,0,(LPTHREAD_START_ROUTINE) Routine,Argument,0,0)
#define MAX_CONTENT 512
#define FILTER_DEVICE_NAME				L"\\\\.\\AsFilterSymbol"
public:
enum DriverMsgType
{
	DvMsg_File,				//文件&进程
	DvMsg_LoadDriver,		//驱动
	DvMsg_CreateAutoRun,	//注册表-自启动
	DvMsg_CreateService,	//注册表-服务项
	DvMsg_ExeIFEO,   //注册表-映像劫持
 DvMsg_Shutdown   //关机			//注册表-映像劫持
};
enum MsgResult
{
	Res_Forbid,
	Res_Pass
};
typedef struct
{
	int DrvPtr;
	int Result;
	int SelfPtr;
	int MessageType;
	int Sign;
	int Value;
	int SubValue;
	int NeedScan;
	wchar_t Content[MAX_CONTENT];
	wchar_t SubContent[MAX_CONTENT];
	wchar_t TransferPath[MAX_CONTENT];
}DRIVEREVENTMSG,*PDRIVEREVENTMSG;
typedef struct
{
	int DrvPtr;
	int Result;
}DRIVERCALLBACKMSG,*PDRIVERCALLBACKMSG;
	DRIVEREVENTMSG Buffer;
	BOOLEAN EventThreadSign;
	HANDLE PipeHandle;
public:
	bool StartProtect();
	bool PauseProtect();
	CProtect();
	~CProtect(void);
private:
	static void EventFilter(CProtect* This);
	
};