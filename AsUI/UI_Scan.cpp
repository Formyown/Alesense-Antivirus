#include "UI_Scan.h"
#include "UI.h"
#include <shlobj.h>
#include <conio.h>
#include "LuaOperation.h"
#include "ListTransport.h"
#include "UI_Protect.h"
#include "UI_Control.h"
void CreateScanInterface()
{
	AsCreateNamedPipe(L"AsUIScan",FileScanCallBack);
}
void CreateForFileScan(UINT Type,LPWSTR URL)
{
	SCAN_UIMESSAGE Msg = {0};
	Msg.Type = Type;
	if (URL!= 0)
	lstrcpy(Msg.URL,URL);
	SendToCoreScan(&Msg,sizeof(SCAN_UIMESSAGE));
}
void SendScanCommand(UINT Id)
{
	SCAN_UIMESSAGE Msg;
	Msg.Type = Id;
	SendToCoreScan(&Msg, sizeof(UINT));
}
void FileScanCallBack(PVOID Data)
{
	PSCAN_VIRINFORMATION SVI = (PSCAN_VIRINFORMATION)Data;
	PSCAN_STATUSINFORMATION    SI = (PSCAN_STATUSINFORMATION)Data;
	if(SVI->Sign == -1) //是Virus_Information
	{
		InformUI_AddScanningResListItem(SVI->URL, SVI->VD.VirusName, L"病毒", SVI->VD.EngineName, NULL);
		Sleep(50);
	}
	else
	{
		if(SI->IsCommand)//是命令
		{
			switch (SI->Command)
			{
			case Finished:
				InformUI_Int(ScanningProgress, 100);
				InformUI(ScanningCompleted);
				break;
			case ManualStop:
				InformUI_Int(ScanningProgress, 100);
				InformUI(ScanningCompleted);
				break;
			case Error:
				InformUI_Int(ScanningProgress, 100);
				InformUI(ScanningCompleted);
				break;
			default:
				break;
			}
		}
		else
		{
		InformUI_Int(TypeOfInformUI::ScanningCount, SI->FileTotal);
		InformUI_Int(TypeOfInformUI::ScanningFoundCount, SI->FindTotal);
		InformUI_Int(TypeOfInformUI::Time, SI->TimeTotal);
		if (SI->TimeTotal)
			InformUI_Int(TypeOfInformUI::ScanningVelocity, SI->FileTotal / SI->TimeTotal);
		InformUI_Int(TypeOfInformUI::ScanningProgress, SI->Progress);
		InformUI_String(TypeOfInformUI::ScanningPath,SI->CurrentURL);
		}
	}
}
bool SelectScanDir(wchar_t* Dir)
{
	BROWSEINFO bi;
	ITEMIDLIST *pidl;

		bi.hwndOwner = NULL;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = Dir;
		bi.lpszTitle = L"选择目录";
		bi.ulFlags = BIF_RETURNONLYFSDIRS;
		bi.lpfn = NULL;
		bi.lParam = 0;
		bi.iImage = 0;
		pidl = SHBrowseForFolder(&bi);
		if (pidl == NULL)
			return false;
		if (!SHGetPathFromIDList(pidl, Dir))
			return false;
		return true;

}
queue<LPWSTR> HandleQueue;
void HandleThread(DWORD32 Counts)
{
	LPWSTR FilePath;
	WCHAR Text[64];
	DWORD32 CurrentCount = 0;
	while (!HandleQueue.empty())
	{
		CurrentCount++;
		wsprintf(Text, L"       正在处理第%d个对象,共%d个....", CurrentCount, Counts);
		InformUI_String(ScanningPath, Text);
		InformUI_Int(ScanningProgress, CurrentCount * 100 / Counts);
		FilePath = HandleQueue.front();
		ForceDeleteFile(FilePath);
		HandleQueue.pop();
	
	}
	ScanningResListClear();
	InformUI(HandleCompleted);
}
void HandleScanningResult()
{
	DWORD32 ResCount = ScanningResListGeCount();
	PSCANNING_RES_LIST_ITEM Item;
	DWORD32 Count = 0;
	for (unsigned int i = 0; i < ResCount; i++)
	{
		Item = ScanningResListGeItem(i);
		if (!Item->Trusted)
		{
			HandleQueue.push(Item->Path);
			Count++;
		}
	}
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HandleThread, (PVOID)Count, 0, 0);
}