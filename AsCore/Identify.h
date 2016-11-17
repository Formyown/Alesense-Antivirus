#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"Psapi.lib")
#include "Core.h"
#pragma comment (lib,"..\\Libs\\AsScanManager.Lib")
#include "..\Libs\AsPipeLib.h"

enum VirusType
{
	Safe,								 //安全
	Virus,								 //病毒文件
	Suspicious,							 //可疑文件
	UnknownFile							 //未知文件
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

typedef void(*PF_SCAN_CALLBACK)(LPWSTR FileName,PVIRUS_DESCRIPTION VD,VirusType VType,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
extern "C" _declspec (dllimport) void InitializeScanManager();
extern "C" _declspec (dllimport) void TerminateScanManager();
extern "C" _declspec (dllimport) PVOID CreateFileIdentify(LPWSTR Path,PF_SCAN_CALLBACK ScanCallBack,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
extern "C" _declspec (dllexport) VirusType IdentifyFile(LPWSTR FileName,PVIRUS_DESCRIPTION VirusDescription,PVOID Info);
class CIdentify
{
private:
	BOOL GetImageRealPath(wchar_t* Dest, wchar_t* OrigPath);
public:
	CIdentify(void);
	~CIdentify(void);
	void GetRealPath(wchar_t* Dest, wchar_t* OrigReg);
	VirusType DirectlyIdentify(LPWSTR FilePath,PVIRUS_DESCRIPTION VD);
	void CallBackIdentify(LPWSTR FilePath, PF_SCAN_CALLBACK CallBack,int CallBackPtr32,int CallBackValue);
	friend void DirScanCallBack(LPWSTR FilePath,LPWSTR DangerName,VirusType Danger,DWORD32 SelfPtr,DWORD32 Reserved2);
};

