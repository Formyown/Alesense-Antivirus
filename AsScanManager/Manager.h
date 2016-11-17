#include <Windows.h>
#include "ManagerTypedef.h"
void RemoveScanTask(int TaskID);
PSCANTASK_LINKEDLIST InsertScanTask(ScanTaskType Type,HANDLE ThreadHandle);
void LoadDllForEngine(const wchar_t* DllPath,const char* FunctionName,PENGINE_INFORMATION EInfo);
void ClearScanTask();
PSCAN_STATUS GetScanTaskStatus(int ID);
DWORD WINAPI NormalScanThread(PVOID arg);
DWORD WINAPI FullScanThread(PVOID arg);
DWORD WINAPI ScanTicketThread(PVOID arg);
/*==================Export======================*/
//extern "C" _declspec (dllexport) 
extern "C" _declspec (dllexport) PVOID CreateScanTask(ScanTaskType Type,wchar_t* Path,PF_SCAN_VIRUS_CALLBACK VirusCallBack,PF_SCAN_FINISH_CALLBACK FinishCallBack,PF_SCAN_ERROR_CALLBACK ErrorCallBack,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
extern "C" _declspec (dllexport) void InitializeScanManager();
extern "C" _declspec (dllexport) void TerminateScanManager();
extern "C" _declspec (dllexport) int GetScanTaskScanedCount(PVOID TaskHandle);
extern "C" _declspec (dllexport) int GetScanTaskVirusCount(PVOID TaskHandle);
extern "C" _declspec (dllexport) int GetScanTaskTicketCount(PVOID TaskHandle);
extern "C" _declspec (dllexport) void SetScanTaskScanStatusToNormal(PVOID TaskHandle);
extern "C" _declspec (dllexport) void SetScanTaskScanStatusToStopped(PVOID TaskHandle);
extern "C" _declspec (dllexport) void SetScanTaskScanStatusToSuspended(PVOID TaskHandle);
extern "C" _declspec (dllexport) bool IsScanTaskScanStatusStopped(PVOID TaskHandle);
extern "C" _declspec (dllexport) bool IsScanTaskScanStatusSuspended(PVOID TaskHandle);
extern "C" _declspec (dllexport) bool IsScanTaskScanStatusNormal(PVOID TaskHandle);
/*==================Export======================*/
extern SCANTASK_LINKEDLIST ScanTask; //…®√Ëπ‹¿Ì¡¥±Ì
typedef int(*JAV_HEUR)(wchar_t* Path,char* Base);
extern ENGINE_INFORMATION Engines[ENGINES_NUM];
extern HANDLE ThreadPool_Scan[SCAN_POOL_MAX];
extern HANDLE ThreadPool_Scan_MainEvent;
extern HANDLE ThreadPool_Scan_ThreadEvent;
extern HANDLE ThreadPool_Scan_QueueEvent;