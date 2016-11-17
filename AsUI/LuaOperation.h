#include <windows.h>
#ifndef _LUA_OPERATION_

#define _LUA_OPERATION_
#include <Windows.h>
#include "bolt\XLUE.h"
#include "bolt\XLGraphic.h"
#include "bolt\XLLuaRuntime.h"

#pragma comment (lib, "lib\\XLUE.lib")

BOOL RegFunctions();
void InformUI(DWORD Funcode);
void InformUI_String(DWORD Funcode, PWCHAR String);
void InformUI_Int(DWORD Funcode, int Int);
void InitMessageTransport();
void FreeMessageTransport();
void DoTransport();
void InformUI_ScanningResultItem(DWORD Funcode, LPCWSTR Name, LPCWSTR Path, LPCWSTR Type, LPCWSTR EngineName, DWORD Trusted, DWORD Selected);
void InformUI_PopWindow(DWORD Funcode, DWORD ID, LPCWCHAR DefaultBtnText, LPCWCHAR OtherBtnText, BOOL IsRightDefault, BOOL IsTick, LPCWCHAR Text1, LPCWCHAR Text2, LPCWCHAR Text3, LPCWCHAR Text4);
void InformUI_Point(DWORD Funcode, int X, int Y);
typedef enum
{
	FullScanning = 0,
	QuickScanning = 1,
	CustomScanning = 2,
	PauseScanning = 3,
	StopScanning = 4,
	ContinueScanning = 5,

	ScanningResultListGetNext = 6,
	ScanningResultListGetPrev = 7,

	DontHandle = 8,
	Handle = 9,
	TrustItem = 10,
	SelectItem = 11,
	MainWindowClose = 18,
	MainWindowMini = 19,
	PopWindowClose = 20
}TypeOfInformControl;

typedef enum
{
	CreatePath = 0,
	CreatedCount = 1,
	FoundCount = 2,
	Version = 3,
	DataBaseVersion = 4,
	ScanningProgress = 5,
	ScanningCount = 6,
	ScanningFoundCount = 7,
	Time = 8,
	StartCustomScanning = 9,
	ScanningPath = 10,
	ScanningResultListAddNext = 11,
	ScanningResultListAddPrev = 12,
	ScanningResultListSetCount = 13,
	ScanningVelocity = 14,
	ScanningCompleted = 15,
	HandleCompleted = 16,
	//	ScanningCompleted = 15,
	SetSettingProtect = 14,
	SetPopWinLocation = 19,
	PopWarningWindow = 20,
	PopInfoWindow = 21
}TypeOfInformUI;
#endif