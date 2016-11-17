#include <Windows.h>
#include "..\Libs\AsPipeLib.h"

#define SendToCoreProtect(Data) AsSendToNamedPipe(L"AsCoreProtect",(PVOID)Data,sizeof(DRIVERCALLBACKMSG))
#define MAX_CONTENT 512
enum DriverMsgType
{
	DvMsg_File,				//文件&进程
	DvMsg_LoadDriver,		//驱动
	DvMsg_CreateAutoRun,	//注册表-自启动
	DvMsg_CreateService,	//注册表-服务项
	DvMsg_ExeIFEO,   //注册表-映像劫持
 DvMsg_Shutdown   //关机			//注册表-映像劫持
};
enum MsgResult
{
	Res_Forbid,
	Res_Pass
};
typedef struct
{
	int DrvPtr;
	int Result;
	int SelfPtr;
	int MessageType;
	int Sign;
	int Value;
	int SubValue;
	int NeedScan;
	wchar_t Content[MAX_CONTENT];
	wchar_t SubContent[MAX_CONTENT];
	wchar_t TransferPath[MAX_CONTENT];
}DRIVEREVENTMSG,*PDRIVEREVENTMSG;
typedef struct
{
	int DrvPtr;
	int Result;
}DRIVERCALLBACKMSG,*PDRIVERCALLBACKMSG;
void OnProtectMessage(PVOID Message);
void CreateProtectInterface();
void ProtectEventFilter();
void HandlePopWindow(PDRIVEREVENTMSG Msg, BOOL Result);