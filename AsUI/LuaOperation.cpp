#include "LuaOperation.h"
#include "UI_Scan.h"
#include "UI_Protect.h"
#include "ListTransport.h"
#include "ListData.h"
#include "UI.h"
#define MSG_RT_NONE 0
#define MSG_RT_INT 1
#define MSG_RT_STRING 2
#define MSG_RT_SCANNING_RES 3
#define MSG_RT_POPWINDOW 4
#define MSG_RT_POINT 5

int MessageListCount = 0;

typedef struct MESSAGE_
{
	DWORD ReservedType;
	DWORD Type;
	PVOID Msg;
}MESSAGE, *PMESSAGE;
typedef struct SCANNING_RES_LIST_MESSAGE_
{
	CHAR Name[MAX_VIRUS_NAME];
	CHAR Path[MAX_PATH];
	CHAR Type[MAX_VIRUS_TYPE];
	CHAR EngineName[MAX_ENGINE_NAME];
	DWORD Trusted;
	DWORD Selected;
}SCANNING_RES_LIST_MESSAGE, *PSCANNING_RES_LIST_PMESSAGE;
typedef struct POPWINDOW_MESSAGE
{
	DWORD Funcode;
	DWORD ID;
	CHAR DefaultBtnText[32];
	CHAR OtherBtnText[32];
	BOOL IsRightDefault;
	BOOL IsTick;
	CHAR Text1[522];
	CHAR Text2[522];
	CHAR Text3[522];
	CHAR Text4[522];
}POPWINDOW_MESSAGE, *PPOPWINDOW_MESSAGE;

typedef struct SPOINT_
{
	int X;
	int Y;
}SPOINT, *PSPOINT;

MESSAGE MessageList[256];
CRITICAL_SECTION CriticalSection;


int InformControl(lua_State* luaState);
int AttachInfoListener(lua_State* luaState);
int LuaGC(lua_State* luaState);
int CreateInstance(lua_State* luaState);

static XLLRTGlobalAPI ObjMemberFunctions[] =
{
	{ "InformControl", InformControl },
	{ "AttachInfoListener", AttachInfoListener },
	{ NULL, NULL }
};


long LuaCallback = NULL;
lua_State* LuaState = NULL;

HANDLE MainThread = NULL;
PVOID __stdcall Instance(PVOID Arg)
{
	static PVOID s_pTheOne = NULL;
	if (s_pTheOne == NULL)
	{
		s_pTheOne = (PVOID)0x0000FFF0;
	}
	return s_pTheOne;
}


BOOL RegFunctions()
{
	XL_LRT_ENV_HANDLE EnvHandle = XLLRT_GetEnv(NULL);
	if (EnvHandle == NULL) return FALSE;
	long res;// = XLLRT_RegisterClass(EnvHandle, "As.Control", ClassMemberFunctions, NULL, 0);


	XLLRTObject theObject;
	theObject.ClassName = "As.Control";
	theObject.MemberFunctions = ObjMemberFunctions;
	theObject.ObjName = "As.Control";
	theObject.userData = NULL;
	theObject.pfnGetObject = (fnGetObject)Instance;

	res = XLLRT_RegisterGlobalObj(EnvHandle, theObject);


	return true;

}
int LuaGC(lua_State* luaState)
{
	return 1;
}
int InformControl(lua_State* luaState)
{
	int Type = lua_tointeger(luaState, 2);
	WCHAR Str[256];
	int StrLen = 0;
	switch (Type)
	{
	case FullScanning:
		CreateForFileScan(File_Full, 0);
		break;
	case QuickScanning:
		CreateForFileScan(File_Quick, 0);
		break;
	case CustomScanning:
		if (SelectScanDir(Str))
		{
			CreateForFileScan(File_Custom, Str);
			InformUI(StartCustomScanning);
		}
			;///取消扫描
		break;
	case StopScanning:
		SendScanCommand(FileScan_Stop);
		break;
	case ContinueScanning:
		SendScanCommand(FileScan_Resume);
		break;
	case PauseScanning:
		SendScanCommand(FileScan_Suspend);
		break;
	case DontHandle:
		ScanningResListClear();
		InformUI(HandleCompleted);
		break;
	case Handle:
		HandleScanningResult();
		break;
	case TrustItem:
		InformControl_ScanningResListSetTrused(lua_tointeger(luaState, 3), lua_tointeger(luaState, 4));
		break;
	case MainWindowClose:
		ShowWindow(g_hWnd, SW_HIDE);
		SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
		break;
	case MainWindowMini:
		ShowWindow(g_hWnd, SW_MINIMIZE);
		break;
	case PopWindowClose:
		DWORD ID = lua_tointeger(luaState, 3);
		BOOL IsDefaultButtonDown = lua_toboolean(luaState, 4);
		HandlePopWindow((PDRIVEREVENTMSG)ID, !IsDefaultButtonDown);
		SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
		break;
	}
	return 1;
}
int AttachInfoListener(lua_State* luaState)
{
	if (!lua_isfunction(luaState, 2))
	{
		return 0;
	}
	LuaCallback = luaL_ref(luaState, LUA_REGISTRYINDEX);
	LuaState = luaState;
	return 1;
}
void InformUI(DWORD Funcode)
{
	EnterCriticalSection(&CriticalSection);
	MessageListCount++;
	PMESSAGE Msg = &MessageList[MessageListCount - 1];
	Msg->Type = Funcode;
	Msg->ReservedType = MSG_RT_NONE;
	LeaveCriticalSection(&CriticalSection);
	return;
}
void InformUI_String(DWORD Funcode, PWCHAR String)
{
	EnterCriticalSection(&CriticalSection);
	MessageListCount++;
	PMESSAGE Msg = &MessageList[MessageListCount - 1];
	Msg->Type = Funcode;
	Msg->ReservedType = MSG_RT_STRING;


	int len = WideCharToMultiByte(CP_UTF8, 0, String, -1, NULL, 0, NULL, NULL);
	if (len == 0)
	{
		MessageListCount--;
		LeaveCriticalSection(&CriticalSection);

		return;
	}
	Msg->Msg = malloc(MAX_PATH);
	WideCharToMultiByte(CP_UTF8, 0, String, -1, (PCHAR)Msg->Msg, len, NULL, NULL);

	LeaveCriticalSection(&CriticalSection);
	return;
}
void InformUI_Int(DWORD Funcode, int Int)
{
	EnterCriticalSection(&CriticalSection);
	MessageListCount++;
	PMESSAGE Msg = &MessageList[MessageListCount - 1];
	Msg->Type = Funcode;
	Msg->Msg = (PVOID)Int;
	Msg->ReservedType = MSG_RT_INT;
	LeaveCriticalSection(&CriticalSection);
	return;
}

void InformUI_Point(DWORD Funcode, int X,int Y)
{
	EnterCriticalSection(&CriticalSection);
	MessageListCount++;
	PMESSAGE Msg = &MessageList[MessageListCount - 1];
	Msg->Type = Funcode;
	Msg->Msg = malloc(sizeof(SPOINT));
	if (Msg->Msg == NULL)
	{
		MessageListCount--;
		LeaveCriticalSection(&CriticalSection);
	}
	PSPOINT Point = (PSPOINT)Msg->Msg;
	Point->X = X;
	Point->Y = Y;
	Msg->ReservedType = MSG_RT_POINT;
	LeaveCriticalSection(&CriticalSection);
	return;
}
void InformUI_ScanningResultItem(DWORD Funcode, LPCWCHAR Name, LPCWCHAR Path, LPCWCHAR Type, LPCWCHAR EngineName, DWORD Trusted, DWORD Selected)
{
	EnterCriticalSection(&CriticalSection);
	MessageListCount++;
	PMESSAGE Msg = &MessageList[MessageListCount - 1];
	Msg->Type = Funcode;
	Msg->ReservedType = MSG_RT_SCANNING_RES;

	int Length[4] = { 0 };
	Length[0] = WideCharToMultiByte(CP_UTF8, 0, Name, -1, NULL, 0, NULL, NULL);
	Length[1] = WideCharToMultiByte(CP_UTF8, 0, Path, -1, NULL, 0, NULL, NULL);
	Length[2] = WideCharToMultiByte(CP_UTF8, 0, Type, -1, NULL, 0, NULL, NULL);
	Length[3] = WideCharToMultiByte(CP_UTF8, 0, EngineName, -1, NULL, 0, NULL, NULL);

	if (Length[0] == 0 || Length[1] == 0 || Length[3] == 0 || Length[4] == 0)
	{
		MessageListCount--;
		LeaveCriticalSection(&CriticalSection);

		return;
	}
	Msg->Msg = malloc(sizeof(SCANNING_RES_LIST_MESSAGE));
	PSCANNING_RES_LIST_PMESSAGE PMsg = (PSCANNING_RES_LIST_PMESSAGE)Msg->Msg;
	WideCharToMultiByte(CP_UTF8, 0, Name, -1, (PCHAR)PMsg->Name, Length[0], NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, Path, -1, (PCHAR)PMsg->Path, Length[1], NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, Type, -1, (PCHAR)PMsg->Type, Length[2], NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, EngineName, -1, (PCHAR)PMsg->EngineName, Length[3], NULL, NULL);
	PMsg->Trusted = Trusted;
	PMsg->Selected = Selected;
	LeaveCriticalSection(&CriticalSection);
	return;
}
//Funcode PopWarningWindow 或 PopInfoWindows
//ID 任意
//默认按钮文字
//其他按钮文字（当只有一个按钮时，这个为NULL）
//IsRightDefault 默认按钮是否在右面
//IsTick 是为默认按钮否倒计时
//文字1（可为NULL）
//文字2（可为NULL）
//文字3（可为NULL）
//文字4（可为NULL）
void InformUI_PopWindow(DWORD Funcode, DWORD ID, LPCWCHAR DefaultBtnText, LPCWCHAR OtherBtnText, BOOL IsRightDefault, BOOL IsTick, LPCWCHAR Text1, LPCWCHAR Text2, LPCWCHAR Text3, LPCWCHAR Text4)
{
	EnterCriticalSection(&CriticalSection);
	MessageListCount++;
	PMESSAGE Msg = &MessageList[MessageListCount - 1];
	Msg->Type = Funcode;
	Msg->ReservedType = MSG_RT_POPWINDOW;

	Msg->Msg = malloc(sizeof(POPWINDOW_MESSAGE));
	if (Msg->Msg == NULL)
	{
		MessageListCount--;
		LeaveCriticalSection(&CriticalSection);
	}
	ZeroMemory((PVOID)Msg->Msg, sizeof(POPWINDOW_MESSAGE));
	PPOPWINDOW_MESSAGE PMsg = (PPOPWINDOW_MESSAGE)Msg->Msg;

	int Length[6] = { 0 };
	Length[0] = WideCharToMultiByte(CP_UTF8, 0, DefaultBtnText, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, DefaultBtnText, -1, PMsg->DefaultBtnText, Length[0], NULL, NULL);
	if (OtherBtnText != NULL)
		Length[1] = WideCharToMultiByte(CP_UTF8, 0, OtherBtnText, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, OtherBtnText, -1, PMsg->OtherBtnText, Length[1], NULL, NULL);
	if (Text1 != NULL)
		Length[2] = WideCharToMultiByte(CP_UTF8, 0, Text1, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, Text1, -1, PMsg->Text1, Length[2], NULL, NULL);
	if (Text2 != NULL)
		Length[3] = WideCharToMultiByte(CP_UTF8, 0, Text2, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, Text2, -1,PMsg->Text2, Length[3], NULL, NULL);
	if (Text3 != NULL)
		Length[4] = WideCharToMultiByte(CP_UTF8, 0, Text3, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, Text3, -1, PMsg->Text3, Length[4], NULL, NULL);
	if (Text4 != NULL)
		Length[5] = WideCharToMultiByte(CP_UTF8, 0, Text4, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, Text4, -1, PMsg->Text4, Length[5], NULL, NULL);

	PMsg->ID = ID;
	PMsg->IsRightDefault = IsRightDefault;
	PMsg->IsTick = IsTick;
	LeaveCriticalSection(&CriticalSection);
	return;
}










void MT_InformUI(DWORD Funcode)
{
	if (LuaCallback == NULL || LuaState == NULL) return;
	int nNowTop = lua_gettop(LuaState);
	lua_rawgeti(LuaState, LUA_REGISTRYINDEX, LuaCallback);
	lua_pushinteger(LuaState, Funcode);
	int nLuaResult = XLLRT_LuaCall(LuaState, 1, 0, L"UIMessageHandler");
	lua_settop(LuaState, nNowTop);
	return;
}
void MT_InformUI_String(DWORD Funcode, PCHAR String)
{
	if (LuaCallback == NULL || LuaState == NULL) return;


	int nNowTop = lua_gettop(LuaState);
	lua_rawgeti(LuaState, LUA_REGISTRYINDEX, LuaCallback);
	lua_pushinteger(LuaState, Funcode);

	lua_pushstring(LuaState, String);
	int nLuaResult = XLLRT_LuaCall(LuaState, 2, 0, L"UIMessageHandler");
	lua_settop(LuaState, nNowTop);
	return;
}
void MT_InformUI_Int(DWORD Funcode, int Int)
{
	if (LuaCallback == NULL || LuaState == NULL) return;
	int nNowTop = lua_gettop(LuaState);
	lua_rawgeti(LuaState, LUA_REGISTRYINDEX, LuaCallback);
	lua_pushinteger(LuaState, Funcode);
	lua_pushinteger(LuaState, Int);
	int nLuaResult = XLLRT_LuaCall(LuaState, 2, 0, L"UIMessageHandler");
	lua_settop(LuaState, nNowTop);
	return;
}
void MT_InformUI_Point(DWORD Funcode, int X, int Y)
{
	if (LuaCallback == NULL || LuaState == NULL) return;
	int nNowTop = lua_gettop(LuaState);
	lua_rawgeti(LuaState, LUA_REGISTRYINDEX, LuaCallback);
	lua_pushinteger(LuaState, Funcode);
	lua_pushinteger(LuaState, X);
	lua_pushinteger(LuaState, Y);
	int nLuaResult = XLLRT_LuaCall(LuaState, 3, 0, L"UIMessageHandler");
	lua_settop(LuaState, nNowTop);
	return;
}
void MT_InformUI_ScanningResultItem(DWORD Funcode, PCHAR Name, PCHAR Path, PCHAR Type, PCHAR EngineName, DWORD Trusted, DWORD Selected)
{
	if (LuaCallback == NULL || LuaState == NULL) return;
	int nNowTop = lua_gettop(LuaState);
	lua_rawgeti(LuaState, LUA_REGISTRYINDEX, LuaCallback);
	lua_pushinteger(LuaState, Funcode);
	lua_pushstring(LuaState, Name);
	lua_pushstring(LuaState, Path);
	lua_pushstring(LuaState, Type);
	lua_pushstring(LuaState, EngineName);
	lua_pushboolean(LuaState, Trusted);
	lua_pushboolean(LuaState, Selected);
	int nLuaResult = XLLRT_LuaCall(LuaState, 7, 0, L"UIMessageHandler");
	lua_settop(LuaState, nNowTop);
	return;
}
void MT_InformUI_PopWindow(DWORD Funcode, DWORD ID, PCHAR DefaultBtnText, PCHAR OtherBtnText, BOOL IsRightDefault, BOOL IsTick, PCHAR Text1, PCHAR Text2, PCHAR Text3, PCHAR Text4)
{
	if (LuaCallback == NULL || LuaState == NULL) return;
	int nNowTop = lua_gettop(LuaState);
	lua_rawgeti(LuaState, LUA_REGISTRYINDEX, LuaCallback);
	lua_pushinteger(LuaState, Funcode);
	if (Funcode == PopWarningWindow)
	{
		lua_pushstring(LuaState, "warning");
	}
	else
	{
		lua_pushstring(LuaState, "info");
	}

	lua_pushinteger(LuaState, ID);
	lua_pushstring(LuaState, DefaultBtnText);
	if (*OtherBtnText != NULL)
		lua_pushstring(LuaState, OtherBtnText);
	else lua_pushnil(LuaState);

	lua_pushboolean(LuaState, IsRightDefault);
	lua_pushboolean(LuaState, IsTick);
	if (*Text1 != NULL)
		lua_pushstring(LuaState, Text1);
	else lua_pushnil(LuaState);

	if (*Text1 != NULL)
		lua_pushstring(LuaState, Text2);
	else lua_pushnil(LuaState);

	if (*Text1 != NULL)
		lua_pushstring(LuaState, Text3);
	else lua_pushnil(LuaState);

	if (*Text1 != NULL)
		lua_pushstring(LuaState, Text4);
	else lua_pushnil(LuaState);

	int nLuaResult = XLLRT_LuaCall(LuaState, 11, 0, L"UIMessageHandler");
	lua_settop(LuaState, nNowTop);
	return;
}









void InitMessageTransport()
{
	
	InitializeCriticalSection(&CriticalSection);
	InitListTransport();
}
void FreeMessageTransport()
{
	
	DeleteCriticalSection(&CriticalSection);
	CloseListTransport();
}
void DoTransport()
{
	EnterCriticalSection(&CriticalSection);
	PMESSAGE Msg;
	PSCANNING_RES_LIST_PMESSAGE PMsg;
	PPOPWINDOW_MESSAGE POPMsg;
	PSPOINT Point;
	DWORD Index = 0;
	while (MessageListCount > 0)
	{

		Msg = &MessageList[Index];
		switch (Msg->ReservedType)
		{
		case MSG_RT_NONE:
			MT_InformUI(Msg->Type);
			break;
		case MSG_RT_INT:
			MT_InformUI_Int(Msg->Type, (int)Msg->Msg);
			break;
		case MSG_RT_STRING:
			MT_InformUI_String(Msg->Type, (PCHAR)Msg->Msg);
			free(Msg->Msg);
			Msg->Msg = NULL;
			break;
		case MSG_RT_SCANNING_RES:
			PMsg = (PSCANNING_RES_LIST_PMESSAGE)Msg->Msg;
			MT_InformUI_ScanningResultItem(Msg->Type, PMsg->Name, PMsg->Path, PMsg->Type, PMsg->EngineName, PMsg->Trusted, PMsg->Selected);
			free(Msg->Msg);
			Msg->Msg = NULL;
			break;
		case MSG_RT_POPWINDOW:
			POPMsg = (PPOPWINDOW_MESSAGE)Msg->Msg;
			MT_InformUI_PopWindow(Msg->Type, POPMsg->ID, POPMsg->DefaultBtnText, POPMsg->OtherBtnText, POPMsg->IsRightDefault, POPMsg->IsTick, POPMsg->Text1, POPMsg->Text2, POPMsg->Text3, POPMsg->Text4);
			free(Msg->Msg);
			break;
		case MSG_RT_POINT:
			Point = (PSPOINT)Msg->Msg;
			MT_InformUI_Point(Msg->Type, Point->X, Point->Y);
			free(Msg->Msg);
			break;
		}
		Index++;
		MessageListCount--;
	}
	LeaveCriticalSection(&CriticalSection);
}