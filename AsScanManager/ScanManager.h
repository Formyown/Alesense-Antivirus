#include <Windows.h>

#pragma once
/*========================================*/
#define StartThread(Routine,Argument) CreateThread(0,0,Routine,Argument,0,0)
#define SCAN_POOL_MAX 20
#define ENGINES_NUM 3
/*==================枚举类型================*/
enum VirusType
{
	Safe,								 //安全
	Virus,								 //病毒文件
	Suspicious,							 //可疑文件
	UnknownFile							 //未知文件
};
enum ScanEngine
{
	JavHeur,							//临时引擎
	SharpArrow,							//SharpArrow启发式
	FileVerify						    //数字签名验证
};
enum Dealingway
{
	Deal_Upload,		//上报
	Deal_Delete,		//删除
	Deal_Fix,			//修复
	Deal_RegDelete,		//删除[注册表]
	Deal_RegFix			//修复[注册表]
};
//=========================================//
typedef	struct _VIRUS_DESCRIPTION_       //病毒描述
{
	wchar_t VirusName[32];				 //病毒名称
	wchar_t EngineName[16];
	DWORD32 SuspiciousValue;				 //可疑度
	DWORD32 HowToDeal;					 //处理方法
}VIRUS_DESCRIPTION, *PVIRUS_DESCRIPTION;

/*==========================================*/
/*==================回调====================*/
typedef void(*PF_SCAN_CALLBACK)(LPWSTR FileName,PVIRUS_DESCRIPTION VD,VirusType VType,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
typedef bool(*SCANENGINE_NORMALCALL)(LPVOID MainArg,PVIRUS_DESCRIPTION VD);
typedef bool(*SCANENGINE_NOINFORCALL)(LPWSTR FileName);
typedef void(*SCANENGINE_INITCALL)();
//========================================//
typedef struct _ENGINE_INFORMATION_
{
	HMODULE hDll;
	SCANENGINE_NORMALCALL FunctionCall;
	SCANENGINE_NOINFORCALL FunctionCall2;
	SCANENGINE_INITCALL Init;
	SCANENGINE_INITCALL Term;
}ENGINE_INFORMATION,*PENGINE_INFORMATION;
/*========================================*/


typedef	struct _SCAN_INFORMATION_		 //扫描时传递的信息
{
  wchar_t Path[MAX_PATH];				 //要扫描的路径
  DWORD32 CallBackPtr32;				 //回调用的信息
  DWORD32 CallBackValue;				 //回调用的信息
  PF_SCAN_CALLBACK  ScanCallBack;
}SCAN_INFORMATION,*PSCAN_INFORMATION;
typedef	struct _SCAN_QUEUE_INFORMATION_		 //队列信息
{
  SCAN_INFORMATION Info;
  PVOID Next;
}SCAN_QUEUE_INFORMATION,*PSCAN_QUEUE_INFORMATION;
/*========================================*/
void LoadDllForEngine(const LPWSTR DllPath,const char* FunctionName,PENGINE_INFORMATION EInfo);
void ClearScanTask();
/*==================Export======================*/
//extern "C" _declspec (dllexport) 
extern "C" _declspec (dllexport) PVOID CreateFileIdentify(LPWSTR Path,PF_SCAN_CALLBACK ScanCallBack,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
extern "C" _declspec (dllexport) void InitializeScanManager();
extern "C" _declspec (dllexport) void TerminateScanManager();
/*==================Export======================*/
extern ENGINE_INFORMATION Engines[ENGINES_NUM];
extern HANDLE ThreadPool_Scan[SCAN_POOL_MAX];
extern HANDLE ThreadPool_Scan_MainEvent;
extern HANDLE ThreadPool_Scan_ThreadEvent;
extern HANDLE ThreadPool_Scan_QueueEvent;