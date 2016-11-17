#include <Windows.h>
#include "Manager.h"
/*========================================*/
extern INT32 IndentifyTaskCount;
extern SCAN_INFORMATION IndentifyTemp; //快速鉴定Temp
extern PSCAN_QUEUE_INFORMATION IndentifyQueue; //快速鉴定队列
extern PSCAN_QUEUE_INFORMATION IndentifyQueueTop; //快速鉴定队列的顶端
/*========================================*/
void FileTraversal(PSCAN_INFORMATION Info);
DWORD WINAPI ScanThreadPool(PVOID arg);
VirusType IndentifyFile(wchar_t* FileName,PVIRUS_DESCRIPTION VirusDescription,PSCAN_INFORMATION Info);
