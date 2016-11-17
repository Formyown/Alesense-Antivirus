===============导出函数====================
PVOID CreateScanTask(ScanTaskType Type,wchar_t* Path,PF_SCAN_VIRUS_CALLBACK VirusCallBack,PF_SCAN_FINISH_CALLBACK FinishCallBack,PF_SCAN_ERROR_CALLBACK ErrorCallBack,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
Type:扫描任务类型(ScanTaskType 枚举见下文附录)
Path:扫描的路径
VirusCallBack :扫描到病毒时回调
FinishCallBack:扫描完毕时的回调
ErrorCallBack :发生错误时的回调
三个回调类型见下文
DWORD32 CallBackPtr32,DWORD32 CallBackValue 2个自定义传入的参数
返回值:创建的任务的句柄
-----------------------------------------------------
void InitializeScanManager();
初始化扫描管理器 (一开始必须调用一次)
-----------------------------------------------------
void TerminateScanManager();
结束任务管理器并释放内存 (最后调用)
-----------------------------------------------------
int GetScanTaskScanedCount(PVOID TaskHandle);
获取扫描任务的已扫描文件数量
TaskHandle:任务的句柄
返回值:已扫描文件数量
-----------------------------------------------------
int GetScanTaskVirusCount(PVOID TaskHandle);
获取扫描任务的已扫描文件发现病毒数量
TaskHandle:任务的句柄
返回值:已扫描文件发现病毒数量
-----------------------------------------------------
void SetScanTaskScanStatusToNormal(PVOID TaskHandle);
设置扫描任务的状态到正常
TaskHandle:任务的句柄
-----------------------------------------------------
void SetScanTaskScanStatusToStopped(PVOID TaskHandle);
设置扫描任务的状态到停止
TaskHandle:任务的句柄
-----------------------------------------------------
void SetScanTaskScanStatusToSuspended(PVOID TaskHandle);
设置扫描任务的状态到暂停
TaskHandle:任务的句柄
-----------------------------------------------------
bool IsScanTaskScanStatusStopped(PVOID TaskHandle);
检测扫描任务的状态是否是停止
TaskHandle:任务的句柄
返回值:任务是否停止
-----------------------------------------------------
bool IsScanTaskScanStatusSuspended(PVOID TaskHandle);
检测扫描任务的状态是否是暂停
TaskHandle:任务的句柄
返回值:任务是否暂停
-----------------------------------------------------
bool IsScanTaskScanStatusNormal(PVOID TaskHandle);
检测扫描任务的状态是否是正常
TaskHandle:任务的句柄
返回值:任务是否正常
===============================================================================
---------------------回调类型--------------------------------
typedef void(*PF_SCAN_VIRUS_CALLBACK)(PVOID TaskHandle,wchar_t* FileName,wchar_t* VirusName,VirusType VType,DWORD32 CallBackPtr32,DWORD32 CallBackValue);
TaskHandle:任务的句柄
FileName:病毒文件名
VirusName:病毒名称
VType:病毒类型(VirusType 枚举见下文附录)
DWORD32 CallBackPtr32,DWORD32 CallBackValue 2个自定义传入的参数
-----------------------------------------------------
typedef void(*PF_SCAN_ERROR_CALLBACK)(PVOID TaskHandle,int ErrorCode);
TaskHandle:任务的句柄
ErrorCode:错误码
-----------------------------------------------------
typedef void(*PF_SCAN_FINISH_CALLBACK)(PVOID TaskHandle ,ScanTaskExitCode ExitCode);
TaskHandle:任务的句柄
ExitCode:退出码(ScanTaskExitCode 枚举见下文附录)
/*=================附录 枚举类型========================*/
enum ScanTaskType
{
	FullScan,							 //全盘扫描
	QuickScan,							 //快速扫描
	SelectScan,							 //指定位置扫描
	QuickIndentify						 //快速鉴定
};
enum VirusType
{
	Safe,								 //安全
	Virus,								 //病毒文件
	Suspicious,							 //可疑文件
	UnknownFile							 //未知文件
};
enum ScanTaskExitCode
{
	Automatic,							//正常退出
	Manual								//手动退出
};