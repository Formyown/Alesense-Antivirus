#include "UI.h"
#include "UI_Protect.h"
#include "UI_Scan.h"
#include "LuaOperation.h"
#include "bolt\XLUE.h"
#include "bolt\XLGraphic.h"
#include "bolt\XLLuaRuntime.h"
#pragma comment (lib, "lib\\XLUE.lib")
#include "resource.h"
NOTIFYICONDATA g_nid = { sizeof(NOTIFYICONDATA) };
HWND g_hWnd;
HMENU g_hTrayMenu;
#define TRAY_MESSAGE WM_USER + 404
WNDPROC OrigWndProc;
BOOL LoadMainXAR()
{
	WCHAR Path[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, Path, MAX_PATH);
	long result = 0;
	WCHAR AbsPath[MAX_PATH] = { 0 };
	wsprintf(AbsPath, L"xar@section://%s|.asui$", Path);
	result = XLFS_MountDir(L"C:\\AleSenseAntivirus\\AleSenseBolt", AbsPath, 0, 0);
	//result = XLUE_AddXARSearchPath(L"E:\\Alesense\\Ale.Sense\\Ale.sense\\AsUI");
	if (result != 0)
	{
		return FALSE;
	}
    result = XLUE_LoadXAR("C:\\AleSenseAntivirus\\AleSenseBolt");
	//result = XLUE_LoadXAR("AleSenseBolt");
	if (result != 0)
	{
		return FALSE;
	}
	return TRUE;
}

int __stdcall LuaErrorHandle(lua_State* luaState, const wchar_t* pExtInfo, const wchar_t* wcszLuaErrorString, PXL_LRT_ERROR_STACK pStackInfo)
{
	static BOOL s_bEnter = FALSE;
	if (!s_bEnter)
	{
		s_bEnter = TRUE;
		MessageBoxW(0, wcszLuaErrorString ? wcszLuaErrorString : L"", L"请反馈此错误", MB_ICONERROR | MB_OK);
		s_bEnter = FALSE;
	}
	return 0;
}

BOOL InitXLUE()
{
	//初始化图形库
	XLGraphicParam param;
	XL_PrepareGraphicParam(&param);
	param.textType = XLTEXT_TYPE_FREETYPE;
	long result = XL_InitGraphicLib(&param);
	result = XL_SetFreeTypeEnabled(TRUE);
	//初始化XLUE,这函数是一个复合初始化函数
	//完成了初始化Lua环境,标准对象,XLUELoader的工作
	result = XLUE_InitLoader(NULL);

	//设置一个简单的脚本出错提示
	XLLRT_ErrorHandle(LuaErrorHandle);

	return TRUE;
}

void UninitXLUE()
{
	//退出流程
	XLUE_Uninit(NULL);
	XLUE_UninitLuaHost(NULL);
	XL_UnInitGraphicLib();
	XLUE_UninitHandleMap(NULL);
}

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	RECT rc;
	DWORD Pid = NULL;
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rc, 0);
	if (!InitXLUE())
	{
		MessageBoxW(NULL, L"侦锐:初始化失败 错误代码0x00000001!", L"错误", MB_OK);
		return 1;
	}

	LoadLibrary(TEXT("core.dll"));
	InitMessageTransport();
	RegFunctions();
	if (!LoadMainXAR())
	{
		MessageBoxW(NULL, L"侦锐:初始化失败 错误代码0x00000002!", L"错误", MB_OK);
		return 1;
	}
	while (Pid != GetCurrentProcessId())
	{
		g_hWnd = FindWindow(NULL, L"侦锐反病毒");
		if (g_hWnd)
			GetWindowThreadProcessId(g_hWnd, &Pid);
	}
	ShowWindow(g_hWnd, SW_HIDE);
	OrigWndProc = (WNDPROC)GetWindowLong(g_hWnd, GWL_WNDPROC);
	SetWindowLong(g_hWnd, GWL_WNDPROC, (LONG)WndProc);
	SetWindowLong(g_hWnd, GWL_STYLE, GetWindowLong(g_hWnd, GWL_STYLE) | WS_SYSMENU);
	g_hTrayMenu = LoadMenu(hinstance, MAKEINTRESOURCE(IDR_TRAYMENU));
	g_hTrayMenu = GetSubMenu(g_hTrayMenu, 0);
	CreateScanInterface();
	CreateProtectInterface();
	
	AddTrayIcon(g_hWnd,hinstance);
	InformUI_Point(TypeOfInformUI::SetPopWinLocation, rc.right - rc.left - 460, rc.bottom - rc.top - 260);
	SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
	MSG msg;
	
	// 主消息循环:
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		DoTransport();
	}
	
	FreeMessageTransport();
	UninitXLUE();
	
	return (int)msg.wParam;
}
void AddTrayIcon(HWND hWnd, HINSTANCE hInst)
{
	g_nid.hWnd = hWnd;
	g_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	g_nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MAINICON));
	g_nid.uCallbackMessage = TRAY_MESSAGE;
	lstrcpy(g_nid.szTip, L"侦锐反病毒");
	Shell_NotifyIcon(NIM_ADD, &g_nid);
}
void RemoveTrayIcon()
{
	g_nid.uFlags = 0;
	Shell_NotifyIcon(NIM_DELETE, &g_nid);
}
void SetTrayText(LPWSTR Text)
{
	lstrcpy(g_nid.szTip, Text);
	Shell_NotifyIcon(NIM_MODIFY, &g_nid);
}
static DWORD lasttick = 0;
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	LRESULT result;
	switch (message)
	{
	case TRAY_MESSAGE:
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			ShowWindow(hwnd, SW_SHOW);
			break;
		case WM_RBUTTONDOWN:
			POINT Point;
			RECT Rect;
			GetCursorPos(&Point);
			SetForegroundWindow(g_hWnd);
			TrackPopupMenu(g_hTrayMenu, TPM_RIGHTBUTTON, Point.x, Point.y, 0, hwnd, NULL);
			break;
		}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_TRAYMENU_MAIN:
			ShowWindow(g_hWnd, SW_SHOW);
			break;
		case ID_TRAYMENU_EXIT:
			RemoveTrayIcon();
			ExitProcess(0);
			break;
		}
	}
		return 0;
		break;
	}
	result = OrigWndProc(hwnd,message,wParam,lParam);
	return result;
}