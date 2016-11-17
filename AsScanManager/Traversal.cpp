#include "Traversal.h"
#include "ScanManager.h"
#include "ScanQueue.h"
CTraversal::CTraversal()
{
	PipeSafeEvent = CreateEvent(NULL,FALSE,TRUE,NULL);
	ZeroMemory(CurrentURL,MAX_PATH);
	Stop = true;
}
bool CTraversal::ScanFile(TraversalType Type,LPWSTR URL)
{
	WCHAR BUFFER[MAX_PATH];
	PTGARG TG = (PTGARG)malloc(sizeof(TGARG));
	ZeroMemory(TG, sizeof(TGARG));
	int DriverType;
	if(!Stop)
		return false;
	FileTotal = 0;
	FindTotal = 0;
	TimeTotal = 0;
	AllFileTotal = 0;
	Progress = 0;
	NeedName = false;
	
	Suspend = false;
	Stop = false;
	TG->This = this;
	switch (Type)
	{
	case CTraversal::File_Quick:
		GetWindowsDirectory(BUFFER,MAX_PATH);
		lstrcpy(TG->Path, BUFFER);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)GetFolderFileCount , TG, 0, 0);
		break;
	case CTraversal::File_Custom:
		lstrcpy(BUFFER,URL);
		lstrcpy(TG->Path, BUFFER);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)GetFolderFileCount, TG, 0, 0);
		break;
	case CTraversal::File_Full:
		TG->FullScan = 1;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)GetFolderFileCount, TG, 0, 0);
		CreateThread(0,0,TraversalClock,this,0,0);
		for (wchar_t i = 65; i < 91; i++)
		{
			BUFFER[0] = i;
			BUFFER[1] = ':';
			BUFFER[2] = '\\';
			BUFFER[3] = '\0';
			DriverType = GetDriveType(BUFFER);
			if (DriverType != 0 && DriverType != 1)
			{
				FileTraversal(BUFFER);
			}
		}
		return true;
		break;
	default:
		break;
	}
	CreateThread(0,0,TraversalClock,this,0,0);
	FileTraversal(BUFFER);
	return true;
}
void Delay(long ms)
{
	long goal = clock() + ms;
	while(goal > clock())
	{
		Sleep(1);
	}
}
DWORD WINAPI TraversalClock(PVOID arg)
{
	CTraversal* This = (CTraversal*)arg;
	
	while(1)
	{
		This->NeedName = true;//要求一份新的当前文件名
		Delay(1000);
		while(This->Suspend)
		{
		Sleep(10);
		if(This->Stop)
			return 0;
		}
		if(This->Stop)
			return 0;
		This->TimeTotal ++;
		This->SendCurrentStatus();

	}
}
void CTraversal::SendCurrentStatus()
{
	CTraversal::SCAN_STATUSINFORMATION SI = { 0 };
	unsigned int NowProgress = (int)((float)FileTotal / (float)AllFileTotal * 100.0);
	SI.Progress = Progress;
	SI.FileTotal = FileTotal;
	SI.FindTotal = FindTotal;
	SI.TimeTotal = TimeTotal;
	if (TimeTotal >= 10 && (NowProgress > Progress))
	{
	Progress = NowProgress;
	SI.Progress = NowProgress;
		}
		else
			if (TimeTotal < 10 && Progress < 10)
				SI.Progress = TimeTotal;
		if (SI.Progress > 100)
			SI.Progress = 100;
		lstrcpy(SI.CurrentURL,CurrentURL);
		WaitForSingleObject(PipeSafeEvent,INFINITE);
		SendScanInformationToUI(&SI,sizeof(CTraversal::SCAN_STATUSINFORMATION));
		SetEvent(PipeSafeEvent);
}
void GetFolderFileCount(CTraversal::PTGARG Dir)
{
	CTraversal::TGARG Info;
	RtlCopyMemory(&Info, Dir,sizeof(CTraversal::TGARG));
	free(Dir);
	if (Info.FullScan)
	{
		WCHAR BUFFER[4];
		int DriverType;
		for (wchar_t i = 65; i < 91; i++)
		{
			BUFFER[0] = i;
			BUFFER[1] = ':';
			BUFFER[2] = '\\';
			BUFFER[3] = '\0';
			DriverType = GetDriveType(BUFFER);
			if (DriverType != 0 && DriverType != 1)
			{
				lstrcpy(Info.Path, BUFFER);
				FileTraversalToGetCount(&Info);
				if (Info.This->Stop)
					return;
			}
		}
		
	}
	else
	{
		FileTraversalToGetCount(&Info);
	}
	
}
void FileTraversalToGetCount(CTraversal::PTGARG Dir)
{
	HANDLE hFile;
	WIN32_FIND_DATA WFD;
	wchar_t Path[MAX_PATH];
	lstrcpy(Path, Dir->Path);
	if (Path[3] != '\0') //不是根目录
	{
		lstrcat(Path, L"\\");
		lstrcat(Dir->Path, L"\\");
	}
	lstrcat(Dir->Path, L"*.*");
	hFile = FindFirstFile(Dir->Path, &WFD);
	if (hFile == INVALID_HANDLE_VALUE) return;
	do
	{
		if (WFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!(lstrcmp(WFD.cFileName, L".") == 0 || lstrcmp(WFD.cFileName, L"..") == 0))
			{
				lstrcpy(Dir->Path, Path);
				lstrcat(Dir->Path, WFD.cFileName);
				FileTraversalToGetCount(Dir);
			}
		}
		else
		{
			if (Dir->This->Stop) goto Close;
			Dir->This->AllFileTotal++;
		}
		
	} while (FindNextFile(hFile, &WFD));
Close:
	FindClose(hFile);
}
void CTraversal::FileTraversal(LPWSTR Dir)
{
	HANDLE hFile;
	WIN32_FIND_DATA WFD;
	VirusType Result;
	VIRUS_DESCRIPTION VD;
	wchar_t Path[MAX_PATH];
	wchar_t FileName[MAX_PATH];
	lstrcpy(Path,Dir);
	if(Path[3] != '\0') //不是根目录
	{
	lstrcat(Path,L"\\");
	lstrcat(Dir,L"\\");
	}
	lstrcat(Path,L"*.*");
	hFile = FindFirstFile(Path,&WFD);
	if(hFile == INVALID_HANDLE_VALUE) return;
	do
	{
		if(WFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(!(lstrcmp(WFD.cFileName,L".")== 0 || lstrcmp(WFD.cFileName,L"..")== 0))
			{
				if (Stop) goto Close;
				lstrcpy(Path,Dir);
				lstrcat(Path,WFD.cFileName);
				FileTraversal(Path);
			}
		}
		else
		{
			if(Stop) goto Close;
			while(Suspend)
			{
				Sleep(50);
			if(Stop) goto Close;
			}
			lstrcpy(FileName,Dir);
			lstrcat(FileName,WFD.cFileName);
			if (NeedName)
			{
				NeedName = false;
				lstrcpy(CurrentURL,FileName);
			}
			
			Result = IdentifyFile(FileName,&VD,0);
			if(Result)
			{
				FindTotal++;
				CTraversal::SCAN_VIRINFORMATION SI ;
				SI.Sign = -1;
				lstrcpy(SI.URL,FileName);
				memcpy(&SI.VD, &VD,sizeof(VIRUS_DESCRIPTION));
				WaitForSingleObject(PipeSafeEvent,INFINITE);
				SendScanInformationToUI(&SI,sizeof(CTraversal::SCAN_VIRINFORMATION));
				SetEvent(PipeSafeEvent);
			}
			FileTotal ++;
		}
	} while (FindNextFile(hFile,&WFD));
Close:
	FindClose(hFile);
}

void CTraversal::ResumeFileTraversal()
{
	Suspend = false;
}
void CTraversal::SuspendFileTraversal()
{
	Suspend = true;
}
void CTraversal::StopFileTraversal()
{
	Stop = true;
}
CTraversal::~CTraversal(void)
{
	CloseHandle(PipeSafeEvent);	
}
DWORD WINAPI FileScanThread(PVOID arg)
{
	struct Arg
		{
			int Type;
			WCHAR Path[MAX_PATH];
		}*ThreadArg = (Arg*)arg;
	if(FileScan != NULL)
	{
		FileScan->StopFileTraversal();
		Sleep(1000);
		delete FileScan;
	}
	FileScan = new CTraversal();
	FileScan->ScanFile((CTraversal::TraversalType)ThreadArg->Type,ThreadArg->Path);
	if(FileScan->Stop == true)//主动关闭
	{
		CTraversal::SCAN_STATUSINFORMATION SI;
		SI.Command = CTraversal::ManualStop;
		SI.IsCommand = true;
		FileScan->SendCurrentStatus();
		SendScanInformationToUI(&SI,sizeof(CTraversal::SCAN_STATUSINFORMATION));
	}
	else
	{
		CTraversal::SCAN_STATUSINFORMATION SI;
		SI.Command = CTraversal::Finished;
		SI.IsCommand = true;
		FileScan->StopFileTraversal();
		FileScan->SendCurrentStatus();
		SendScanInformationToUI(&SI,sizeof(CTraversal::SCAN_STATUSINFORMATION));
	}

	free(arg);
	return 0;
}
void OnUIScanMessage(PVOID Data)
{
	CTraversal::PSCAN_UIMESSAGE Msg = (CTraversal::PSCAN_UIMESSAGE)Data;
	if(Msg->Type >= 0xf)
	switch (Msg->Type)
	{
	case CTraversal::FileScan_Stop:
		FileScan->StopFileTraversal();
		break;
	case CTraversal::FileScan_Suspend: 
		FileScan->SuspendFileTraversal();
		break;
	case CTraversal::FileScan_Resume:
		FileScan->ResumeFileTraversal();
		break;
	}
	else
	{
		struct Arg
		{
			int Type;
			WCHAR Path[MAX_PATH];
		}*ThreadArg = (Arg*)malloc(sizeof(Arg));
		ThreadArg->Type = Msg->Type;
		if(Msg->Type == CTraversal::File_Custom)
			lstrcpy(ThreadArg->Path,Msg->URL);
		CreateThread(0,0,FileScanThread,ThreadArg,0,0);
	}
}