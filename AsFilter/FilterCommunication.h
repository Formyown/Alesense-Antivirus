#include "FilterEntry.h"
#pragma once
NTSTATUS IoControl(PDEVICE_OBJECT DriverObject,PIRP Irp);
BOOLEAN PassEvent(LPWSTR Content,LPWSTR SubContent,DWORD32 MsgType,DWORD32 SubValue,DWORD32 Value,DWORD32 NeedScan);
void InitlizeCommunication();
#define MAX_CONTENT 512
//=============================DriverControlCode=========================================//
#define CTL_WAITFOREVENT (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN,0xAF0,METHOD_BUFFERED,FILE_ALL_ACCESS)
#define CTL_STARTPROTECTION (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN,0xAF1,METHOD_BUFFERED,FILE_ALL_ACCESS)
#define CTL_STOPPROTECTION (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN,0xAF2,METHOD_BUFFERED,FILE_ALL_ACCESS)
#define CTL_EVENTCALLBACK (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN,0xAF3,METHOD_BUFFERED,FILE_ALL_ACCESS)
#define CTL_INSERTPROTECTPID (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN,0xAF4,METHOD_BUFFERED,FILE_ALL_ACCESS)
#define CTL_REMOVEPROTECTPID (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN,0xAF5,METHOD_BUFFERED,FILE_ALL_ACCESS)
//=======================================================================================//
enum DriverMsgType
{
	DvMsg_File,				//文件&进程
	DvMsg_LoadDriver,		//驱动
	DvMsg_CreateAutoRun,	//注册表-自启动
	DvMsg_CreateService,	//注册表-服务项
	DvMsg_ExeIFEO,			//注册表-映像劫持
	DvMsg_Shutdown   //关机			//关机
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
	
}DRIVEREVENTMSG,*PDRIVEREVENTMSG;
typedef struct _DRIVEREVENTQUEUE
{
	LIST_ENTRY ListEntry;
	DRIVEREVENTMSG Event;
}DRIVEREVENTQUEUE,*PDRIVEREVENTQUEUE;
typedef struct _DRIVERCALLBACKMSG
{
	DWORD32 DrvPtr;
	DWORD32 Result;
}DRIVERCALLBACKMSG,*PDRIVERCALLBACKMSG;
extern PDRIVEREVENTMSG DriverEventSpace;
extern BOOLEAN IsOfflineMode;