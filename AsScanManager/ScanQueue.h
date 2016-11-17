#include <Windows.h>
#include "ScanManager.h"
/*========================================*/
extern INT32 IdentifyTaskCount;
extern SCAN_INFORMATION IdentifyTemp; //快速鉴定Temp
extern PSCAN_QUEUE_INFORMATION IdentifyQueue; //快速鉴定队列
extern PSCAN_QUEUE_INFORMATION IdentifyQueueTop; //快速鉴定队列的顶端
/*========================================*/
extern "C" _declspec (dllexport) VirusType IdentifyFile(LPWSTR FileName,PVIRUS_DESCRIPTION VirusDescription,PSCAN_INFORMATION Info);
DWORD WINAPI ScanThreadPool(PVOID arg);
