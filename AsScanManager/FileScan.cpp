#include "FileScan.h"
INT32 IndentifyTaskCount = 0;
SCAN_INFORMATION IndentifyTemp; //快速鉴定Temp
PSCAN_QUEUE_INFORMATION IndentifyQueue = NULL; //快速鉴定队列
PSCAN_QUEUE_INFORMATION IndentifyQueueTop = NULL; //快速鉴定队列的顶端
void FileTraversal(PSCAN_INFORMATION Info)
{
	HANDLE hFile;
	WIN32_FIND_DATA WFD;
	VIRUS_DESCRIPTION VirusDescription;
	PSCAN_STATUS Status = &Info->TaskHandle->Status;
	VirusType Result;
	wchar_t Path[MAX_PATH];
	wchar_t FileName[MAX_PATH];
	lstrcpy(Path,Info->Path);
	if(Path[3] != '\0') //不是根目录
	{
	lstrcat(Info->Path,L"\\");
	lstrcat(Path,L"\\");
	}
	lstrcat(Info->Path,L"*.*");
	hFile = FindFirstFile(Info->Path,&WFD);
	if(hFile == INVALID_HANDLE_VALUE) return;
	do
	{
		if(WFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(!(lstrcmp(WFD.cFileName,L".")== 0 || lstrcmp(WFD.cFileName,L"..")== 0))
			{
				lstrcpy(Info->Path,Path);
				lstrcat(Info->Path,WFD.cFileName);
				FileTraversal(Info);
			}
		}
		else
		{
			if(Status->ScanStatus == ScanTaskStatus::Stoped) goto Close;
			while(Status->ScanStatus == ScanTaskStatus::Suspended)
			{
				Sleep(50);
			if(Status->ScanStatus == ScanTaskStatus::Stoped) goto Close;
			}
			lstrcpy(FileName,Path);
			lstrcat(FileName,WFD.cFileName);
			Result = IndentifyFile(FileName,&VirusDescription,Info);
			Status->ScanedCount ++;
			if(Result)
			{
				Status->VirusCount ++;
				
			}
		}
	} while (FindNextFile(hFile,&WFD));
Close:
	FindClose(hFile);
}
DWORD WINAPI ScanThreadPool(PVOID arg)
{
	SCAN_INFORMATION IndentifyInformation;
	VIRUS_DESCRIPTION VirusDescription;
	PSCAN_QUEUE_INFORMATION Queue;
	BOOLEAN IsQueueTask = false;
	VirusType VType;
	while(1)
	{
	WaitForSingleObject(ThreadPool_Scan_ThreadEvent,INFINITE);
	memmove(&IndentifyInformation,&IndentifyTemp,sizeof(SCAN_INFORMATION));
	SetEvent(ThreadPool_Scan_MainEvent);
Deal:
	//=================
	VType = IndentifyFile(IndentifyInformation.Path,&VirusDescription,&IndentifyInformation);
	//=================
	if(!IsQueueTask)
	IndentifyTaskCount --;
	else
	IsQueueTask = false;
	if(IndentifyQueue != NULL)
	{
		WaitForSingleObject(ThreadPool_Scan_QueueEvent,INFINITE);
		if(IndentifyQueue != NULL)
		{
		Queue = IndentifyQueue;
		IndentifyQueue = (PSCAN_QUEUE_INFORMATION)IndentifyQueue->Next;//改变头
		if(IndentifyQueue == NULL)
			IndentifyQueueTop = NULL;
		memmove(&IndentifyInformation,&Queue->Info,sizeof(SCAN_INFORMATION));
		free(Queue);
		IsQueueTask = true;
		SetEvent(ThreadPool_Scan_QueueEvent);//让其他线程获取队列信息
		goto Deal; //跳转到处理过程
		}
	}
	}
}
VirusType IndentifyFile(wchar_t* FileName,PVIRUS_DESCRIPTION VirusDescription,PSCAN_INFORMATION Info)
{
	bool VirusFlag = false;
	
	if(Engines[HeurDet].FunctionCall(FileName))
	{
		VirusFlag = true;
		lstrcpy(VirusDescription->VirusName,L"HEUR\\Malware.Gen01");
	}
	
	if(VirusFlag)
	{
		if(!Engines[FileSDVer].FunctionCall(FileName))
		{
		Info->VirusCallBack(Info->TaskHandle,FileName,VirusDescription->VirusName,VirusType::Virus,Info->CallBackPtr32,Info->CallBackValue);
	return VirusType::Virus;
		}
	}
	Info->VirusCallBack(Info->TaskHandle,FileName,0,VirusType::Safe,Info->CallBackPtr32,Info->CallBackValue);
	return VirusType::Safe;
}