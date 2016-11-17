#include <Windows.h>
#include <stdio.h>
#include <WinInet.h>
#include "AsCRC32.h"
#pragma comment(lib, "Wininet.lib")
#define DLSIZE_MAX 1024*64
typedef void (*UPDATE_PROGRESS_CALLBACK)(char* CurrentFile,DWORD32 FileNumberTotal,DWORD32 CurrentFileDownLoadedSize,DWORD32 CurrentFileNumber);
DWORD WINAPI DownLoadFileByList(char* FileName);
DWORD32 DownloadFileToDisk(char * url,char * Destination,UPDATE_PROGRESS_CALLBACK OnProgressCallBack,DWORD32 FileNumberTotal,DWORD32 CurrentFileNumber);
void OnDownLoadProgress(char* CurrentFile,DWORD32 FileNumberTotal,DWORD32 CurrentFileDownLoadedSize,DWORD32 CurrentFileNumber);
void CleanLine(char* str);
int GetLineValue(FILE* File);