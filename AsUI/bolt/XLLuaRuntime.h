/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, LTD. 2009                     =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   XLLuaRuntime.h
*   Author      :   xlue group(xlue@xunlei.com)
*   Create      :   2009/01/04
*   LastChange  :   
*   History     :	
*								
*   Description :   XLLuaRuntime.dll的头文件，定义了相关的接口
*					目前除了ENV的调用是线程安全的外，其余的接口都不是
*					线程安全的，请在主线程中使用
********************************************************************/  

#ifndef _XUNLEI_LUA_RUNTIME_H_
#define _XUNLEI_LUA_RUNTIME_H_

#ifndef LUA_BUILD_AS_DLL
	#define LUA_BUILD_AS_DLL
#endif // LUA_BUILD_AS_DLL

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include "lua/lua.h"
#include "lua/lauxlib.h"

#ifdef __cplusplus
}
#endif // __cplusplus


#ifdef XLLUA_RUNTIME_EXPORTS
	#ifdef __cplusplus 
		#define XL_LRT_API(x) extern "C" __declspec(dllexport) x __stdcall 
	#else
		#define XL_LRT_API(x) __declspec(dllexport) x __stdcall 
	#endif //__cplusplus
#elif defined(XLUE_UNION)
	#ifdef __cplusplus
		#define XL_LRT_API(x) extern "C" x __stdcall
	#else
		#define XL_LRT_API(x) x __stdcall
	#endif //__cplusplus
#else // not XLLUA_RUNTIME_EXPORTS
	#ifdef __cplusplus
		#define XL_LRT_API(x) extern "C" __declspec(dllimport) x __stdcall
	#else
		#define XL_LRT_API(x) __declspec(dllimport) x __stdcall
	#endif //__cplusplus
#endif // XLLUA_RUNTIME_EXPORTS


//关于缩写:
//XL = Xunlei
//LRT = Lua RunTime

//---使用---
//创建LUA环境
//LUA环境->创建LUA运行时
//创建LUA code chunk
//将code chunk放到LUA运行时中运行

typedef void* XL_LRT_ENV_HANDLE;
typedef void* XL_LRT_RUNTIME_HANDLE;
typedef void* XL_LRT_CHUNK_HANDLE;
typedef void* XL_LRT_HOOK_HANDLE;
typedef void* XL_LRT_DEBUG_LOGS;

typedef void* (__stdcall *fnGetObject)(void* userData);

typedef struct tagXLLRTGlobalAPI {
	const char *name;
	lua_CFunction func;
	unsigned long permission;
} XLLRTGlobalAPI;

struct tagXLLRTObject
{
    fnGetObject pfnGetObject;
    void* userData;
	const char* ObjName;
	const char* ClassName;
	XLLRTGlobalAPI* MemberFunctions;
    unsigned long Permission;
};
typedef struct tagXLLRTObject XLLRTObject;

struct tagXLLRTClass
{
	const char* className;
	const char* fahterClassName;
	XLLRTGlobalAPI* MemberFunctions;
	unsigned long permission;
};
typedef struct tagXLLRTClass XLLRTClass;

typedef struct XL_LRT_ERROR_STACK{
	XL_LRT_DEBUG_LOGS logs;
	struct {
		unsigned short top;
		unsigned long topsix;
		unsigned long all;
	}hash;
}*PXL_LRT_ERROR_STACK;
typedef int (__stdcall *fnLuaErrorHandle)(lua_State* luaState,const wchar_t* pExtInfo,
										  const wchar_t* luaErrorString,PXL_LRT_ERROR_STACK pStackInfo);

typedef long (__stdcall *fnGlobalSetCallback) (lua_State* luaState,const char* globalObjID,void* udata);

//-- 一些参数类型定义 --
#define XLLRT_HISTORY_TYPE_ERROR 0

#define XLLRT_CHUNK_TYPE_STRING 0
#define XLLRT_CHUNK_TYPE_FILE   1
#define XLLRT_CHUNK_TYPE_MODULE 2

//debug参数
#define XLLRT_DEBUG_TYPE_HOOK		0
#define XLLRT_DEBUG_TYPE_NOHOOK		1
#define XLLRT_DEBUG_TYPE_DUMPSTATE  2

#define XLLRT_DEBUG_MAX_LOG_LEN		1024
#define XLLRT_DEBUG_CALL_LOG_COUNT	1024
#define XLLRT_DEBUG_STACK_LOG_COUNT	100
#define XLLRT_DEBUG_MAX_NAME_LEN	128

// 所有的错误码定义
#define XLLRT_RESULT_SUCCESS 0
#define XLLRT_RESULT_ENV_EXIST 1
#define XLLRT_RESULT_ENV_NOT_FOUND 2
#define XLLRT_RESULT_ENV_INVALID 3
#define XLLRT_RESULT_CHUNK_EXIST 4
#define XLLRT_RESULT_CHUNK_NOT_FOUND 5
#define XLLRT_RESULT_CHUNK_CODE_LENGTH_NOT_ALLOW 6
#define XLLRT_RESULT_CHUNK_COMPLIED 7
#define XLLRT_RESULT_CANNOT_CREATE_LUASTATE 8
#define XLLRT_RESULT_CHUNK_INVALID 9
#define XLLRT_RESULT_CHUNK_NEED_ID 10
#define XLLRT_RESULT_CHUNK_COMPLIE_ERROR 11
#define XLLRT_RESULT_RUNTIME_RUN_ERROR 12
#define XLLRT_RESULT_RUNTIME_NOT_READY 13
#define XLLRT_RESULT_RUNTIME_HAVE_INIT 14
#define XLLRT_RESULT_RUNTIME_INVALID 15
#define XLLRT_RESULT_RUNTIME_EXIST 16
#define XLLRT_RESULT_RUNTIME_NOT_FOUND 17
#define XLLRT_RESULT_FUNCTION_EXIST 18
#define XLLRT_RESULT_FUNCTION_NOT_FOUND 19
#define XLLRT_RESULT_GLOBAL_OBJ_INVALID 20
#define XLLRT_RESULT_GLOBAL_OBJ_EXIST 21
#define XLLRT_RESULT_GLOBAL_OBJ_NOT_FOUND 22
#define XLLRT_RESULT_CLASS_NOT_FOUND 23
#define XLLRT_RESULT_FILE_NOT_FOUND 24
#define XLLRT_RESULT_CLASS_EXIST    25
#define XLLRT_RESULT_NO_PERMISSION 26
#define XLLRT_RESULT_CHUNK_MOUDLE_RUN 27
#define XLLRT_RESULT_DEBUG_BUFNOTENOUGH 30
#define XLLRT_RESULT_PARAM_INVALID 31

//获取Xunlei Runtime的版本号
XL_LRT_API(unsigned long) XLLRT_GetVersion();

//---LUA环境---
//环境包括
//1) 由XLLuaRuntime扩展的一些lua api
//2) 在运行脚本前 ,往 lua 脚本里注册的对象,以及lua脚本可在运行过程中动态取得的对象
//3) lua里#include 其他.lua文件的路径管理
//4) lua错误事件与lua错误历史纪录
//5) lua脚本创建的 public object
//pstrEnvName == NULL 或则 strlen(pstrEnvName) == 0 都表示创建/获取 默认Env
XL_LRT_API(XL_LRT_ENV_HANDLE) XLLRT_CreateEnv(const wchar_t* pstrEnvName);
XL_LRT_API(long) XLLRT_DestroyEnv(const wchar_t* pEnvName);

XL_LRT_API(XL_LRT_ENV_HANDLE) XLLRT_GetEnv(const wchar_t* pstrEnvName);
XL_LRT_API(unsigned long) XLLRT_AddRefEnv(XL_LRT_ENV_HANDLE hEnv);
XL_LRT_API(unsigned long) XLLRT_ReleaseEnv(XL_LRT_ENV_HANDLE hEnv);

//Env内的Global API,Global Obj管理
XL_LRT_API(long) XLLRT_RegisterGlobalAPI(XL_LRT_ENV_HANDLE hEnv,XLLRTGlobalAPI theAPI);
XL_LRT_API(long) XLLRT_RemoveGlobalAPI(XL_LRT_ENV_HANDLE hEnv,const char* theAPIName);
XL_LRT_API(BOOL) XLLRT_IsGlobalAPIRegistered(XL_LRT_ENV_HANDLE hEnv,const char* theAPIName);

XL_LRT_API(long) XLLRT_RegisterGlobalObj(XL_LRT_ENV_HANDLE hEnv,XLLRTObject theObj);
XL_LRT_API(long) XLLRT_RemoveGlobalObj(XL_LRT_ENV_HANDLE hEnv,const char* objName);
XL_LRT_API(long) XLLRT_RegisterGlobalSetCallback(XL_LRT_ENV_HANDLE hEnv,fnGlobalSetCallback pfnCallback,void* udata);
XL_LRT_API(long) XLLRT_IsGlobalObjRegistered(XL_LRT_ENV_HANDLE hEnv,const char* objName);

//支持单继承
XL_LRT_API(long) XLLRT_RegisterClass(XL_LRT_ENV_HANDLE hEnv,const char* className,XLLRTGlobalAPI* MemberFunctions,const char* fahterClassName,unsigned long permission);
XL_LRT_API(long) XLLRT_UnRegisterClass(XL_LRT_ENV_HANDLE hEnv,const char* className);
XL_LRT_API(long) XLLRT_DoRegisterClass(const char* className,lua_State* luaState);
XL_LRT_API(BOOL) XLLRT_IsClassRegistered(XL_LRT_ENV_HANDLE hEnv, const char* className);

// 枚举相关接口
// 返回的枚举器是当前env的一个快照，只可以在一个线程里面使用！
typedef void* XL_LRT_ENUMERATOR;

/*
使用示例
XL_LRT_ENUMERATOR hEnum = XLLRT_BeginEnumGlobalAPI(hEnv);
XLLRTGlobalAPI luaAPI;
while(XLLRT_GetNextGlobalAPI(hEnum, &luaAPI))
{
	TSINFO(L"name=%S, function=0x%x", luaAPI.name, luaAPI.func);
}
XLLRT_EndEnum(hEnum);
*/
XL_LRT_API(XL_LRT_ENUMERATOR) XLLRT_BeginEnumGlobalAPI(XL_LRT_ENV_HANDLE hEnv);
XL_LRT_API(BOOL) XLLRT_GetNextGlobalAPI(XL_LRT_ENUMERATOR hEnum, XLLRTGlobalAPI* lpGlobalAPI);

/*
使用示例
XL_LRT_ENUMERATOR hEnum = XLLRT_BeginEnumGlobalObject(hEnv);
XLLRTObject luaObject;
while(XLLRT_GetNextGlobalObject(hEnum, &luaObject))
{
	TSINFO(L"class=%S, objname=%S", luaObject.ClassName, luaObject.ObjName);
	const XLLRTGlobalAPI* lpAPI = luaObject.MemberFunctions;
	while (lpAPI->name != NULL)
	{
		TSINFO(L"name=%S, func=0x%x", lpAPI->name, lpAPI->func);
		++lpAPI;
	}
}
XLLRT_EndEnum(hEnum);
*/
XL_LRT_API(XL_LRT_ENUMERATOR) XLLRT_BeginEnumGlobalObject(XL_LRT_ENV_HANDLE hEnv);
XL_LRT_API(BOOL) XLLRT_GetNextGlobalObject(XL_LRT_ENUMERATOR hEnum, XLLRTObject* lpGlobalObj);

/*
使用示例
XL_LRT_ENUMERATOR hEnum = XLLRT_BeginEnumGlobalClass(hEnv);
XLLRTClass luaClass;
while(XLLRT_GetNextGlobalClass(hEnum, &luaClass))
{
	TSINFO(L"class=%S, fatherclass=%S", luaClass.className, luaClass.fahterClassName);
	const XLLRTGlobalAPI* lpAPI = luaClass.MemberFunctions;
	while (lpAPI->name != NULL)
	{
		TSINFO(L"name=%S, func=0x%x", lpAPI->name, lpAPI->func);
		++lpAPI;
	}
}
XLLRT_EndEnum(hEnum);
*/
XL_LRT_API(XL_LRT_ENUMERATOR) XLLRT_BeginEnumGlobalClass(XL_LRT_ENV_HANDLE hEnv);
XL_LRT_API(BOOL) XLLRT_GetNextGlobalClass(XL_LRT_ENUMERATOR hEnum, XLLRTClass* lpGlobalClass);

// 枚举结束后，调用该接口来终结所使用的枚举器
// 枚举器终结后，返回的class、api和obj，里面的所有字符串、memberfunction指针都会失效！
XL_LRT_API(BOOL) XLLRT_EndEnum(XL_LRT_ENUMERATOR hEnum);


// push一个对象到lua栈
XL_LRT_API(long) XLLRT_PushXLObject(lua_State* luaState,const char* className,void* pRealObj);

//--LUA 运行时---
// 对luaState的包裹，允许使用协程
// 一个luaruntime必须自始至终运行在同一个线程
// 一个env可以创建一个或者多个runtime
XL_LRT_API(XL_LRT_RUNTIME_HANDLE) XLLRT_CreateRunTime(XL_LRT_ENV_HANDLE hEnv,const char* id,unsigned long permission);
XL_LRT_API(long) XLLRT_DestroyRunTime(XL_LRT_ENV_HANDLE hEnv, const char* pID);

XL_LRT_API(XL_LRT_RUNTIME_HANDLE) XLLRT_GetRuntime(XL_LRT_ENV_HANDLE hEnv,const char* pRuntimeID);
XL_LRT_API(unsigned long) XLLRT_GetRuntimePermission(XL_LRT_RUNTIME_HANDLE hRuntime);
XL_LRT_API(unsigned long) XLLRT_ReleaseRunTime(XL_LRT_RUNTIME_HANDLE hRunTime);
XL_LRT_API(unsigned long) XLLRT_AddRefRunTime(XL_LRT_RUNTIME_HANDLE hRunTime);
XL_LRT_API(XL_LRT_ENV_HANDLE) XLLRT_GetOwnerEnv(XL_LRT_RUNTIME_HANDLE hRunTime);
XL_LRT_API(const char*) XLLRT_GetRuntimeID(XL_LRT_RUNTIME_HANDLE hRuntime);
XL_LRT_API(lua_State*) XLLRT_GetLuaState(XL_LRT_RUNTIME_HANDLE hRunTime);
XL_LRT_API(XL_LRT_RUNTIME_HANDLE) XLLRT_GetRuntimeFromLuaState(lua_State* luaState);

XL_LRT_API(size_t) XLLRT_GetAllLuaState(lua_State** luaState, size_t nCount );
XL_LRT_API(const wchar_t*) XLLRT_AddLoadLuaFile(const wchar_t* lpLuaFile);
XL_LRT_API(size_t) XLLRT_GetLoadLuaFileCount();
XL_LRT_API(long) XLLRT_GetLoadLuaFileByIndex(int nIndex, wchar_t *wstrFilePath, int nLen);

//LuaCodeChunk的变化
//LuaCodeChunk用来表示一个Lua Function,通过LuaCodeChunk,你可以得到一个lua function,或则简单的直接运行它
//LuaCodeChunk的name不再具有id的作用,如果需要持有，复用，由使用者管理
//共有3种方法创建lua chunk
//1) 指定对应的lua源文件的路径
//2) 指定对应的lua模块的路径,以及需要的函数名
//3) 内存中的代码片段

//创建Chunk只是创建Chunk对象，并纪录其类型,源位置! lua代码的合法性要到RunChunk或则GetChunkFunc的时候才生效
XL_LRT_API(unsigned long) XLLRT_AddRefChunk(XL_LRT_CHUNK_HANDLE hChunk);
XL_LRT_API(unsigned long) XLLRT_ReleaseChunk(XL_LRT_CHUNK_HANDLE hChunk);

XL_LRT_API(const wchar_t*) XLLRT_GetChunkName(XL_LRT_CHUNK_HANDLE hChunk);
XL_LRT_API(unsigned long) XLLRT_GetChunkType(XL_LRT_CHUNK_HANDLE hChunk);

//先支持最简单的string类型的CodeChunk, pCodeBuffer为明文的LUA脚本,编码为UTF-8
XL_LRT_API(unsigned long) XLLRT_CreateChunk(const wchar_t* pstrName,const unsigned char* pCodeBuffer,unsigned long len,XL_LRT_CHUNK_HANDLE* pResult);

// code chunk对应一个lua文件
XL_LRT_API(unsigned long) XLLRT_CreateChunkFromFile(const wchar_t* pstrName,const wchar_t* path,XL_LRT_CHUNK_HANDLE* pResult);

//code chunk对应module中的一个函数
XL_LRT_API(unsigned long) XLLRT_CreateChunkFromModule(const wchar_t* pstrName,const wchar_t* modulePath,const char* func,XL_LRT_CHUNK_HANDLE* pResult);

//在环境中执行该Chunk.
XL_LRT_API(long) XLLRT_RunChunk(XL_LRT_RUNTIME_HANDLE hRunTime,XL_LRT_CHUNK_HANDLE hChunk);

//得到Chunk对应的lua function,压入lua stack的顶部。
XL_LRT_API(long) XLLRT_PrepareChunk(XL_LRT_RUNTIME_HANDLE hRunTime,XL_LRT_CHUNK_HANDLE hChunk);

// 调用lua栈顶的函数
XL_LRT_API(long) XLLRT_LuaCall(lua_State* luaState,int args,int results,const wchar_t* contextInfo);

// 重要！设置一个错误回调函数，在luacall出现脚本错误后，会回调该函数
XL_LRT_API(long) XLLRT_ErrorHandle(fnLuaErrorHandle  pfnErrorHandle);

// 获取当前luastate产生的最后一条错误信息
XL_LRT_API(const char*) XLLRT_GetLastError(lua_State* luaState);

// HOOK相关函数
XL_LRT_API(XL_LRT_HOOK_HANDLE) XLLRT_SetGlobalAPIHook(XL_LRT_ENV_HANDLE hEnv, const char *name, lua_CFunction func, BOOL pre);
XL_LRT_API(BOOL) XLLRT_RemoveGlobalAPIHook(XL_LRT_ENV_HANDLE hEnv, const char* name, XL_LRT_HOOK_HANDLE hook);

XL_LRT_API(XL_LRT_HOOK_HANDLE) XLLRT_SetGlobalObjectFunctionHook(XL_LRT_ENV_HANDLE hEnv, const char* objName, const char* name, lua_CFunction func, BOOL pre);
XL_LRT_API(BOOL) XLLRT_RemoveGlobalObjectFunctionHook(XL_LRT_ENV_HANDLE hEnv, const char* objName, const char* name, XL_LRT_HOOK_HANDLE hook);

XL_LRT_API(XL_LRT_HOOK_HANDLE) XLLRT_SetClassFunctionHook(XL_LRT_ENV_HANDLE hEnv, const char* className, const char* name, lua_CFunction func, BOOL pre);
XL_LRT_API(BOOL) XLLRT_RemoveClassFunctionHook(XL_LRT_ENV_HANDLE hEnv, const char* className, const char* name, XL_LRT_HOOK_HANDLE hook);

//debug interface
//mem pool && logs
typedef void* XL_LRT_DEBUG_MEMPOOL;

XL_LRT_API(long) XLLRT_DebugLogsPopNextLog(const char**pLog, XL_LRT_DEBUG_LOGS logs);
XL_LRT_API(long) XLLRT_DebugMemPoolGetLogs(const XL_LRT_DEBUG_MEMPOOL MemPool,XL_LRT_DEBUG_LOGS logs);
XL_LRT_API(long) XLLRT_DebugCreateStackMemPool(XL_LRT_DEBUG_MEMPOOL* pMemPool);
XL_LRT_API(long) XLLRT_DebugDesroyStackMemPool(XL_LRT_DEBUG_MEMPOOL MemPool);
XL_LRT_API(long) XLLRT_DebugCreateLogs(XL_LRT_DEBUG_LOGS* pLogs);
XL_LRT_API(long) XLLRT_DebugDestroyLogs(XL_LRT_DEBUG_LOGS logs);
//init 
XL_LRT_API(long) XLLRT_DebugInit(const char DebugeeName[],long dwDbgType);
XL_LRT_API(long) XLLRT_DebugGetDebugeeName(char* pBuf);
XL_LRT_API(long) XLLRT_DebugGetType(long* pType);
//logs 
XL_LRT_API(long) XLLRT_DebugGetCallLog(XL_LRT_DEBUG_MEMPOOL* pMemPool);
XL_LRT_API(long) XLLRT_DebugGetStackLog(XL_LRT_DEBUG_MEMPOOL* pMemPool);
XL_LRT_API(long) XLLRT_DebugGetProcessCallLog(const char DebugeeName[],XL_LRT_DEBUG_MEMPOOL* pMemPool);
XL_LRT_API(long) XLLRT_DebugGetProcessStackLog(const char DebugeeName[],XL_LRT_DEBUG_MEMPOOL* pMemPool);
//no logs
XL_LRT_API(long) XLLRT_DebugOutputLuaStack(lua_State* pState,XL_LRT_DEBUG_MEMPOOL MemPool);
XL_LRT_API(long) XLLRT_DebugGetCurState(lua_State** ppState);
XL_LRT_API(long) XLLRT_DebugGetProcessCurState(const char DebugeeName[],lua_State** ppState);
XL_LRT_API(long) XLLRT_DebugDestroyState(lua_State* pState);

//for minidump
//call back in writedump, inputed prama type should be PVOID, const PMINIDUMP_CALLBACK_INPUT, const PMINIDUMP_CALLBACK_OUTPUT
XL_LRT_API(int) XLLRT_DebugGetDumpList(void* CallbackParam,const void* CallbackInput,
										  const void*CallbackOutput);
//for read lua state from dump file , the second inputed praram type should be PMINIDUMP_MEMORY_LIST
XL_LRT_API(long) XLLRT_DebugGetStateFromDump(void* pDumpData,void* pMemList,lua_State**ppState);

// 高级功能，获取函数的真正地址
XL_LRT_API(lua_CFunction) XLLRT_GetFunctionAddress( lua_CFunction lpFun );

XL_LRT_API(long) XLLRT_Stat(long type);

#endif //_XUNLEI_LUA_RUNTIME_H_