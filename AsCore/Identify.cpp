#include "Identify.h"

CIdentify::CIdentify(void)
{
	InitializeScanManager();
}


CIdentify::~CIdentify(void)
{
	TerminateScanManager();
}

VirusType CIdentify::DirectlyIdentify(LPWSTR FilePath,PVIRUS_DESCRIPTION VD)
{
	return IdentifyFile(FilePath,VD,0);
}
void CIdentify::CallBackIdentify(LPWSTR FilePath, PF_SCAN_CALLBACK CallBack,int CallBackPtr32,int CallBackValue)
{
	CreateFileIdentify(FilePath,CallBack,CallBackPtr32,CallBackValue);
}
void CIdentify::GetRealPath(wchar_t* Dest, wchar_t* OrigReg)
{
	HKEY hKey;
	long ret;
	wchar_t ServiceName[64];
	wchar_t ServiceImagePath[MAX_PATH * 4];
	int find;
	HANDLE hFile = 0;
	DWORD Regtype = REG_EXPAND_SZ;//定义数据类型
	DWORD Pathsize = MAX_PATH*4;//定义数据长度
	WCHAR sourceReg[MAX_PATH];
	lstrcpy(sourceReg, OrigReg);
	if (OrigReg[1] != 'R')
	{
		GetImageRealPath(Dest, OrigReg);
		return;
	}
	for (find = lstrlen(sourceReg); find>0;)
	{
		if (sourceReg[find] == '\\')break;
		find--;
	}

	lstrcpy(ServiceName, &sourceReg[find + 1]);
	lstrcpy(ServiceImagePath, &sourceReg[18]); //去掉开头多余字符 \REGISTRY\Machine\ 
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, ServiceImagePath, 0, KEY_READ, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		lstrcpy(Dest, ServiceName);
		return;
	}
	ret = RegQueryValueEx(hKey, L"ImagePath", NULL, &Regtype, (LPBYTE)ServiceImagePath, &Pathsize); //先复制到serviceimagepath 处理后复制到dest
	RegCloseKey(hKey);
	if (ret != ERROR_SUCCESS)
	{
		lstrcpy(Dest, ServiceName);
		return;
	}
	if (GetImageRealPath(Dest, ServiceImagePath) == FALSE)
		lstrcpy(Dest, L"未知");
	else
		if (PathFileExists(Dest) == FALSE)
			lstrcpy(Dest, L"文件不存在");
	return;
}
BOOL CIdentify::GetImageRealPath(wchar_t* Dest, wchar_t* OrigPath)
{

	wchar_t* PathTemp;
	wchar_t Buffer[MAX_PATH];
	WCHAR sourcePath[MAX_PATH];
	lstrcpy(sourcePath, OrigPath);
	if (sourcePath[0] == '\0')
	{
		lstrcpy(Dest, L"无");
		return TRUE;
	}
	PathTemp = (wchar_t*)wcsstr(sourcePath, L"SystemRoot");
	if (PathTemp != NULL)
	{
		if (wcsstr(PathTemp, L"SystemRoot%") != NULL)
			PathTemp = PathTemp + sizeof(wchar_t) * 11;
		else
			PathTemp = PathTemp + sizeof(wchar_t) * 10;
		GetWindowsDirectory(Buffer, MAX_PATH);
		lstrcpy(Dest, Buffer);
		lstrcat(Dest, L"\\");
		lstrcat(Dest, PathTemp);
		return TRUE;
	}
	if (wcsstr(sourcePath, L":") == NULL)
	{
		GetWindowsDirectory(Buffer, MAX_PATH);
		lstrcpy(Dest, Buffer);
		lstrcat(Dest, L"\\");
		lstrcat(Dest, sourcePath);
		return TRUE;
	}
	if (wcsstr(sourcePath, L"\\??\\") != NULL)
		lstrcpy(Dest, sourcePath + 4);
	else
	{
		lstrcpy(Dest, sourcePath);
		return FALSE;
	}
	return TRUE;
}
void GetProcessPathByPID(wchar_t* dst, DWORD32 PID)
{
	HANDLE h_Process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID);
	if (h_Process)
	{
		//GetProcessImageFileNameA(h_Process,dst,1024);
		GetModuleFileNameEx((HMODULE)h_Process, 0, dst, MAX_PATH);
		CloseHandle(h_Process);
		return;
	}
	_itow_s(PID, dst, 10,10);

}
