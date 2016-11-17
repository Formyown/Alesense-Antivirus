#pragma once
#include <Windows.h>
#include <time.h>
#include "ScanManager.h"
#include "..\Libs\AsPipeLib.h"

class CTraversal
{
#define SendScanInformationToUI(Data,Size) AsSendToNamedPipe(L"AsUIScan",Data,Size)
public:
typedef struct
{
	int Type;
	WCHAR URL[MAX_PATH];
}SCAN_UIMESSAGE,*PSCAN_UIMESSAGE;
	enum TraversalType
	{
		File_Quick,
		File_Custom,
		File_Full
	};
	enum UIScanMessageType
	{
		FileScan_Stop = 0xf,
		FileScan_Suspend,
		FileScan_Resume
	};
	enum ScanInformationCommand
	{
		Finished,ManualStop,Error
	};
	typedef struct
	{
		CTraversal* This;
		UINT FullScan;
		WCHAR Path[MAX_PATH];
	}TGARG,*PTGARG;
	typedef	struct
	{
		unsigned int FileTotal;
		unsigned int FindTotal;
		unsigned int TimeTotal;
		unsigned int Progress;
		WCHAR CurrentURL[MAX_PATH];
		bool IsCommand;
		unsigned int Command; //0 == Sign = // 1 == command
	}SCAN_STATUSINFORMATION,*PSCAN_STATUSINFORMATION;
	typedef	struct
	{
		unsigned int Sign;		//用来兼容SCAN_INFORMATION 总为-1
		unsigned int Reserved;	//Reserved
		unsigned int Reserved2;	
		WCHAR URL[MAX_PATH];	//病毒路径
		VIRUS_DESCRIPTION VD;
	}SCAN_VIRINFORMATION,*PSCAN_VIRINFORMATION;
public:
	CTraversal();
	~CTraversal(void);
	bool ScanFile(TraversalType,LPWSTR);
	void ResumeFileTraversal();
	void SuspendFileTraversal();
	void StopFileTraversal();
	void SendCurrentStatus();
	bool Stop;
	bool Suspend;
private:
	HANDLE PipeHandle;
	HANDLE PipeSafeEvent;
	void FileTraversal(LPWSTR Path);
	bool NeedName;
	unsigned int FileTotal;
	unsigned int FindTotal;
	unsigned int TimeTotal;
	unsigned int AllFileTotal;
	unsigned int Progress;
	friend void FileTraversalToGetCount(PTGARG Path);
	WCHAR CurrentURL[MAX_PATH];
	friend DWORD WINAPI TraversalClock(PVOID);
	friend void GetFolderFileCount(PTGARG Dir);
};
void OnUIScanMessage(PVOID Data);
DWORD WINAPI FileScanThread(PVOID arg);
static CTraversal* FileScan;
