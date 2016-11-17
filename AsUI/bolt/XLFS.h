
/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, Ltd. 2004-2011                =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   XLFS.h
*   Author      :   xlue group(xlue@xunlei.com)
*   Create      :   2010-1-28
*   LastChange  :   
*   History     :	
*
*   Description :   迅雷虚拟文件系统接口定义
*                   考虑到系统兼容的问题，XLFS里用到的路径都是大小写敏感的 
*                   并且在路径中请尽量使用Linux的/
*                   注意线程安全问题,XLFS提供的接口都应该是线程安全的
*                   有计划支持异步IO,所以接口都有保留
*
********************************************************************/ 

#ifndef __XLFS_H__
#define __XLFS_H__

#ifdef XLFS_EXPORTS
	#ifdef __cplusplus
		#define XLFS_API(x) extern "C" __declspec(dllexport) x __stdcall 
	#else
		#define XLFS_API(x) __declspec(dllexport) x __stdcall 
	#endif //__cplusplus
#elif defined (XLUE_UNION)
	#ifdef __cplusplus
		#define XLFS_API(x) extern "C"  x __stdcall 
	#else
		#define XLFS_API(x) x __stdcall 
	#endif //__cplusplus
#else // XLFS_EXPORTS
	#ifdef __cplusplus
		#define XLFS_API(x) extern "C" __declspec(dllimport) x __stdcall 
	#else
		#define XLFS_API(x) __declspec(dllimport) x __stdcall 
	#endif //__cplusplus
#endif // XLFS_EXPORTS

// xlfs句柄的定义
typedef void * XLFS_FILE_HANDLE;
typedef void * XLFS_DIR_HANDLE;
typedef void * XLFS_PROTOCOLLOADER_HANDLE;
typedef void * XLFS_MEDIUMLOADER_HANDLE;
typedef void * XLFS_STREAM_HANDLE;


#define MAX_ABSPATH_LENGTH 512


typedef int (*pfnFileOpenCallBack)(void *pParam,XLFS_FILE_HANDLE hFile,int result);
typedef int (*pfnDirOpenCallBack)(void *pParam,XLFS_DIR_HANDLE hDir,int result);
typedef int (*pfnFileUpdateCallBack)(void *pParam);

// 自定义的解密回调函数
typedef int (*pfnDecryptCallBack)(void* userData, const void* lpContext, __int64 contextLen, void* lpDecryptBuffer, __int64 decryptBufferOffset, __int64 decryptBufferLen); 

typedef struct tagFileOpenCallBackData
{
	pfnFileOpenCallBack callBackFunc; //回调函数
	void * pParam; //用来储存this指针
}FileOpenCallBackData, *LPFileOpenCallBackData;

typedef struct tagFileUpdateCallBackData
{
	pfnFileUpdateCallBack callBackFunc; //回调函数
	void * pParam; //用来储存this指针
}FileUpdateCallBackData, *LPFileUpdateCallBackData;

typedef struct tagDirOpenCallBackData
{
	pfnDirOpenCallBack callBackFunc; //回调函数
	void * pParam; //用来储存this指针
}DirOpenCallBackData;

typedef struct tagXLFSDirEntry
{  
	unsigned long Attributes; //暂时未定义属性的意义 
	__int64 CreationTime;  
	__int64 LastAccessTime;  
	__int64 LastWriteTime;  
	__int64 FileSize;//DIR没有size
	wchar_t Name[MAX_PATH];  
} XLFSDirEntry;

// xlfs错误码的定义
#define XLFS_RESULT_SUCCESS        0
#define XLFS_RESULT_ASYN_OPENING   1
#define XLFS_RESULT_FILE_NOT_FOUND 2
#define XLFS_RESULT_NOT_OPEN       3
#define XLFS_RESULT_PATH_ERROR     4
#define XLFS_RESULT_PROTOCOL_ERROR 5
#define XLFS_RESULT_OPENMODE_NOT_SUPPORTED 6

#define XLFS_RESULT_DIR_NOT_FOUND 7
#define XLFS_RESULT_INVALID_PARAM 8
#define XLFS_RESULT_NOT_XAR_FILE 9
#define XLFS_RESULT_BLOCK_MEDIA 10
#define XLFS_RESULT_NOT_SUPPORTED	11
#define XLFS_RESULT_NOT_FILE	12
#define XLFS_RESULT_NOT_DIR		13
#define XLFS_RESULT_MEDIUM_NOT_SUPPORTED	14
#define XLFS_RESULT_LOAD_MODULE_FAILED	15
#define XLFS_RESULT_LOAD_RESOURCE_NOT_FOUND	16
#define XLFS_RESULT_LOAD_LOAD_RESOURCE_FAILED	17
#define XLFS_RESULT_LOAD_INVALID_PEFILE	18
#define XLFS_RESULT_LOAD_INVALID_SECITON	19

#define XLFS_RESULT_INVLAID_HANDLE -1

// 初始化和反初始化相关函数
XLFS_API(long) XLFS_Init();
XLFS_API(long) XLFS_Uninit();

// 设置自定义的解密回调函数
XLFS_API(long) XLFS_SetXARDecryptCallBack(int flags, pfnDecryptCallBack lpCallBack, void* userData);

// 文件系统相关
XLFS_API(long) XLFS_FileExists(const wchar_t * filePath);
XLFS_API(long) XLFS_DirExists(const wchar_t* dirPath);

// 路径处理函数
XLFS_API(long) XLFS_FixPath(const wchar_t* pSrcPath,wchar_t** ppDestPath);

//
// 文件相关操作,相关事件定义
// 注意，只有文件打开是异步的(异步均未实现)
//
//openMode继承自C,包括r,w,r+等
XLFS_API(long) XLFS_OpenFile(const wchar_t * filePath, const wchar_t * openMode, XLFS_FILE_HANDLE *phFile,LPFileOpenCallBackData pCallBack);
XLFS_API(__int64) XLFS_ReadFile(XLFS_FILE_HANDLE hFile, unsigned char *pBuffer, __int64 bufferLen);
XLFS_API(__int64) XLFS_WriteFile(XLFS_FILE_HANDLE hFile, const unsigned char *pBuffer, __int64 bufferLen);
XLFS_API(__int64) XLFS_SeekFile(XLFS_FILE_HANDLE hFile,__int64 newPosition,int origin);
XLFS_API(__int64) XLFS_GetFileSize(XLFS_FILE_HANDLE hFile);
XLFS_API(__int64) XLFS_GetFilePosition(XLFS_FILE_HANDLE hFile);
XLFS_API(long) XLFS_DeleteFile(const wchar_t * filePath);
XLFS_API(long) XLFS_IsEOF(XLFS_FILE_HANDLE hFile);
XLFS_API(long) XLFS_CloseFile(XLFS_FILE_HANDLE hFile);
XLFS_API(long) XLFS_GetFileAbsPath(XLFS_FILE_HANDLE hFile,const wchar_t** ppResult);

XLFS_API(long) XLFS_GetXARFileType(XLFS_FILE_HANDLE hFile, unsigned long* lpType);

//
// 目录相关操作,主要用于遍历
//
XLFS_API(long) XLFS_OpenDirectory(const wchar_t * dirPath, XLFS_DIR_HANDLE *phDir,DirOpenCallBackData* pCallback);
XLFS_API(long) XLFS_ReadDirectory(XLFS_DIR_HANDLE hDir,XLFSDirEntry* pResult);
XLFS_API(long) XLFS_CloseDirectory(XLFS_DIR_HANDLE hDir);

//
//Mount相关操作
//
XLFS_API(long) XLFS_MountDir(const wchar_t* pDestPath,const wchar_t* pSrcPath,long mountOption,const wchar_t* mountCmd);
XLFS_API(long) XLFS_MountFile(const wchar_t* pDestPath,const wchar_t* pSrcPath,long mountOption,const wchar_t* mountCmd);
XLFS_API(long) XLFS_UnMount(const wchar_t* pDestPath,long unmountOption);
XLFS_API(long) XLFS_GetRealAbsPath(const wchar_t* pVirtualPath, int nIndex, wchar_t** ppResult);
XLFS_API(long) XLFS_GetRealAbsPathCount(const wchar_t* pVirtualPath, int *pnCount);

//
//文件Cache相关操作
//

//vaildTime说明了该cache的有效时间(单位ms)。设置为0使用系统默认值，-1为永远有效
XLFS_API(XLFS_FILE_HANDLE) XLFS_UpdateCacheFile(const wchar_t * filePath,long vaildTime);
XLFS_API(long) XLFS_GetCacheFile(const wchar_t * fileAbsPath, XLFS_FILE_HANDLE *phFile);

// 释放一段时间内不使用的资源，减少内存占用
XLFS_API(long) XLFS_CacheGC(); 

//立刻让一个cachefile无效,对该cachefile的下一次Update操作将会触发一次文件IO
XLFS_API(long) XLFS_RemoveCacheFile(const wchar_t * filePath);

//设定xar使用的加密协议
XLFS_API(long) XLFS_SetXARProtocol(short version, unsigned long mask);

// 统计信息接口
XLFS_API(long) XLFS_Stat(long type);

// 流操作相关接口
XLFS_API(XLFS_STREAM_HANDLE) XLFS_CreateStreamFromFile(const wchar_t* fileName, const wchar_t* lpMode);
XLFS_API(XLFS_STREAM_HANDLE) XLFS_CreateStreamFromMountFile(const wchar_t* fileName, const wchar_t* lpMode);
XLFS_API(XLFS_STREAM_HANDLE) XLFS_CreateStreamFromMemory(void* lpBuffer, unsigned long size);
XLFS_API(XLFS_STREAM_HANDLE) XLFS_CreateStreamFromResource(HMODULE hModule, const wchar_t* resName, const wchar_t* resType);

XLFS_API(unsigned long) XLFS_AddRefStream(XLFS_STREAM_HANDLE hStream);
XLFS_API(unsigned long) XLFS_ReleaseStream(XLFS_STREAM_HANDLE hStream);

XLFS_API(BOOL) XLFS_StreamSeek(XLFS_STREAM_HANDLE hStream, int offset, int origin);
XLFS_API(unsigned int) XLFS_StreamRead(XLFS_STREAM_HANDLE hStream, void* buffer, unsigned int size);
XLFS_API(unsigned int) XLFS_StreamWrite(XLFS_STREAM_HANDLE hStream, const void* buffer, unsigned int size);
XLFS_API(unsigned int) XLFS_StreamGetPosition(XLFS_STREAM_HANDLE hStream);
XLFS_API(unsigned int) XLFS_StreamSetPosition(XLFS_STREAM_HANDLE hStream, unsigned int pos);
XLFS_API(unsigned int) XLFS_StreamGetSize(XLFS_STREAM_HANDLE hStream);
XLFS_API(unsigned int) XLFS_StreamSetSize(XLFS_STREAM_HANDLE hStream, unsigned int size);

#endif // __XLFS_H__