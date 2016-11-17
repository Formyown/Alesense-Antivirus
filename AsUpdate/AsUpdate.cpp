#include "AsUpdate.h"
#define GetNextLine(Buffer,Length,File) fgets(Buffer,Length,File);CleanLine(Buffer);
typedef struct 
{
	int a;
	int b;
	wchar_t c[4096];
	wchar_t d[4096];
}st,*pst;
int main(int argc, char* argv[])
{
	//int FileSize;
	//CreateThread(0,0,(LPTHREAD_START_ROUTINE)DownLoadFileByList,"update.txt",0,0);
	system("pause");
	
	return 0;
}

DWORD WINAPI DownLoadFileByList(char* FileName)
{
	char Tag[64] = {0};
	char Url[1024] = {0};
	char Path[MAX_PATH] = {0};
	char Crc32[16] = {0};
	int Version = 0;
	int Tried = 0;
	DWORD32 FileNumberTotal,CurrentFileNumber = 0;
	BOOLEAN Flag = TRUE;
	FILE* File;
	DWORD32 CRC32;
	fopen_s(&File,FileName,"r");
	if(File == NULL)
		return FALSE;

	Version = GetLineValue(File);
	FileNumberTotal = GetLineValue(File);
	while(Flag)
	{
		Tried = 0;
		CurrentFileNumber ++;
		fscanf_s(File,"%s %s",Url,Crc32);
		fseek(File,1,SEEK_CUR);
		GetNextLine(Path,sizeof(Path),File)
ReDownLoad:
		CRC32 = DownloadFileToDisk(Url,Path,OnDownLoadProgress,FileNumberTotal,CurrentFileNumber);
		if(CRC32 != strtol(Crc32,NULL,16))
		{
			if(CRC32 == NULL)
			Flag = FALSE;
			else
			{
				Tried ++;
				if(Tried >= 2)
				{
					Flag = FALSE;
					break;
				}
			printf("所下载的文件 %s Crc32不符!重新下载.....\n",Path);
				goto ReDownLoad; //如果CRC32 不符 重新下载
			}
		}
		if(feof(File))
			break;
		memset(Crc32,0,sizeof(Crc32));
		memset(Url,0,sizeof(Url));
		memset(Path,0,sizeof(Path));
	}
	if(Flag)
	printf("\n下载完毕!");
	else
	printf("\n下载失败!");
	return Flag;
}
void CleanLine(char* str)
{
	if(str[strlen(str)-1] == '\n')
		str[strlen(str)-1] = '\0';
}
int GetLineValue(FILE* File)
{
	char Tag[64];
	GetNextLine(Tag,sizeof(Tag),File);
	return atoi(strstr(Tag,"=")+1);
}
DWORD32 DownloadFileToDisk(char * url,char * Destination,UPDATE_PROGRESS_CALLBACK OnProgressCallBack,DWORD32 FileNumberTotal,DWORD32 CurrentFileNumber)
{
	char buffer[DLSIZE_MAX] = {0};
	BOOLEAN Result = FALSE;
	DWORD bytes_read_total = 0;
    DWORD bytes_read;
	DWORD Timeout = 1000;
	FILE* File;
	DeleteFileA(Destination);
	fopen_s(&File,Destination,"ab+");
	if(File == NULL)
	return FALSE;
    HINTERNET hInternet = InternetOpen(L"AsUpdate", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
    if(hInternet == NULL)
    return FALSE;
    HINTERNET TargetHandle = InternetOpenUrlA(hInternet,url,NULL,0,INTERNET_FLAG_RELOAD,0);
	InternetSetOption(NULL, INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT, &Timeout, sizeof(DWORD));
	if(TargetHandle == NULL)
	{
		InternetCloseHandle(hInternet);
		return FALSE;
	}
    while(InternetReadFile(TargetHandle, buffer, DLSIZE_MAX, &bytes_read))
	{
		if(bytes_read == 0)
		{
			Result = TRUE;
			break;
		}
		bytes_read_total += bytes_read;
		fwrite(buffer,bytes_read,1,File);
		OnDownLoadProgress(Destination,FileNumberTotal,bytes_read_total,CurrentFileNumber);
	}
	fclose(File);
	InternetCloseHandle(TargetHandle);
    InternetCloseHandle(hInternet);
	return Result?GetFileCrc32(Destination):Result;
}
void OnDownLoadProgress(char* CurrentFile,DWORD32 FileNumberTotal,DWORD32 CurrentFileDownLoadedSize,DWORD32 CurrentFileNumber)
{
		//system("cls");
		printf("正在下载:%s 进度:%d\\%d 已经下载: %d Bytes\n",CurrentFile,FileNumberTotal,CurrentFileNumber,CurrentFileDownLoadedSize);
}