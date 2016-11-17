#include <Windows.h>
#pragma once
/*========================================*/
#define StartThread(Routine,Argument) CreateThread(0,0,Routine,Argument,0,0)
#define SCAN_POOL_MAX 20
#define ENGINES_NUM 2
/*==================枚举类型================*/
enum ScanTaskType
{
	FullScan,							 //全盘扫描
	QuickScan,							 //快速扫描
	SelectScan,							 //指定位置扫描
	QuickIndentify						 //快速鉴定
};
enum VirusType
{
	Safe,								 //安全
	Virus,								 //病毒文件
	Suspicious,							 //可疑文件
	UnknownFile							 //未知文件
};
enum ScanTaskStatus
{
	Normal,								 //正常的
	Suspended,							 //暂停的
	Stoped,								 //停止的
	Error = 0x004						 //错误的
};
enum ScanTaskExitCode
{
	Automatic,							//自动关闭
	Manual								//手动关闭
};
enum ScanEngine
{
	HeurDet,							//启发式
	FileSDVer						    //数字签名
};
/*==================回调====================*/
typedef void(*PF_SCAN_VIRUS_CALLBACK)(PVOID TaskHandle,wchar_t* FileName,wchar_t* VirusName,VirusType VType,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
typedef void(*PF_SCAN_ERROR_CALLBACK)(PVOID TaskHandle,int ErrorCode);
typedef void(*PF_SCAN_FINISH_CALLBACK)(PVOID TaskHandle ,ScanTaskExitCode ExitCode);
typedef bool(*SCANENGINE_NORMALCALL)(wchar_t* FileName);
typedef void(*SCANENGINE_INITCALL)();
/*==========================================*/
typedef	struct _SCAN_STATUS_
{
  ScanTaskStatus ScanStatus;		     //扫描状态(in & out)
  int ScanedCount;						 //已扫描的文件数
  int VirusCount;						 //病毒数
  int ErrorCode;						 //错误码
  unsigned int TimeUsed;				 //已使用的时间
}SCAN_STATUS,*PSCAN_STATUS;
//========================================//
typedef struct _ENGINE_INFORMATION_
{
	HMODULE hDll;
	SCANENGINE_NORMALCALL FunctionCall;
	SCANENGINE_INITCALL Init;
	SCANENGINE_INITCALL Term;
}ENGINE_INFORMATION,*PENGINE_INFORMATION;
/*========================================*/
typedef	struct _SCANTASK_LINKEDLIST_
{
  int ID;
  ScanTaskType Type;                     //扫描类型
  SCAN_STATUS  Status;					 //扫描状态
  HANDLE ThreadHandle;					 //线程句柄
  void *NextTask;						 //链表指针
}SCANTASK_LINKEDLIST,*PSCANTASK_LINKEDLIST;
/*========================================*/
typedef	struct _VIRUS_DESCRIPTION_       //病毒描述
{					 
  wchar_t VirusName[32];				 //病毒名称
  DWORD32 SuspiciousValue;				 //可疑度
}VIRUS_DESCRIPTION,*PVIRUS_DESCRIPTION;

/*==========================================*/
typedef	struct _SCAN_INFORMATION_		 //扫描时传递的信息
{
  wchar_t Path[MAX_PATH];				 //要扫描的路径
  PSCANTASK_LINKEDLIST TaskHandle;						 //扫描任务的Handle
  DWORD32 CallBackPtr32;				 //回调用的信息
  DWORD32 CallBackValue;				 //回调用的信息
  PF_SCAN_VIRUS_CALLBACK  VirusCallBack;
  PF_SCAN_FINISH_CALLBACK FinishCallBack;
  PF_SCAN_ERROR_CALLBACK ErrorCallBack;
}SCAN_INFORMATION,*PSCAN_INFORMATION;
typedef	struct _SCAN_QUEUE_INFORMATION_		 //队列信息
{
  SCAN_INFORMATION Info;
  PVOID Next;
}SCAN_QUEUE_INFORMATION,*PSCAN_QUEUE_INFORMATION;
/*========================================*/
