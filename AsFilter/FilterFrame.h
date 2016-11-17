#pragma once
#include "FilterEntry.h"
#include "FilterCommunication.h"
#include "FilterDasm.h"
/*==========================================*/
#define SYSCALL_INDEX(ServiceFunction) (*(PDWORD32)((PUCHAR)ServiceFunction + 1))
#define SYSCALL_FUNCTION_ADDR(ServiceFunction) KeServiceDescriptorTable->ntoskrnl.ServiceTableBase[SYSCALL_INDEX(ServiceFunction)]
#define SYSCALL_FUNCTION_ADDR_BY_INDEX(Index) KeServiceDescriptorTable->ntoskrnl.ServiceTableBase[Index]
#define SHADOW_ADDR_BYINDEX(Index) KeServiceDescriptorTableShadow->win32k.ServiceTableBase[Index-0x1000]

/*==========================================*/

BOOLEAN LoadFrame();
void UnLoadFrame();
void LoadProxyService();
void LoadProxyHardCodeService();
void StartHookRecovery();
void StopHookRecovery();
BOOLEAN SysEnterHookInline();
BOOLEAN UnSysEnterHookInline();
void KiFastCallInline();
void KiFastCallInlineXP();
void GetShadowTableAddress();
int _memcmp(const void *buffer1, const void *buffer2, int count);
void HookRecoveryRoutine(__in struct _KDPC *Dpc,__in_opt PVOID DeferredContext,__in_opt PVOID SystemArgument1,__in_opt PVOID SystemArgument2);
extern BOOLEAN Sign_StopFilter;
DWORD32 KiFastCallFilter(DWORD32 ServiceId,UINT_PTR OriginalAddress,PUINT_PTR BaseAddr);
/*==========================================*/
typedef struct _KSYSTEM_SERVICE_TABLE
 {
    PDWORD32  ServiceTableBase;          // SSDT (System Service Dispatch Table)的基地址
    PDWORD32  ServiceCounterTableBase;   // 用于 checked builds, 包含 SSDT 中每个服务被调用的次数
	DWORD32   NumberOfService;           // 服务函数的个数, NumberOfService * 4 就是整个地址表的大小
	DWORD32   ParamTableBase;            // SSPT(System Service Parameter Table)的基地址
} KSYSTEM_SERVICE_TABLE, *PKSYSTEM_SERVICE_TABLE;
typedef struct _KSERVICE_TABLE_DESCRIPTOR
{
	KSYSTEM_SERVICE_TABLE   ntoskrnl;  // ntoskrnl.exe 的服务函数
	KSYSTEM_SERVICE_TABLE   win32k;    // win32k.sys 的服务函数(GDI32.dll/User32.dll 的内核支持)
	KSYSTEM_SERVICE_TABLE   notUsed1;
	KSYSTEM_SERVICE_TABLE   notUsed2;
	
} KSERVICE_TABLE_DESCRIPTOR, *PKSERVICE_TABLE_DESCRIPTOR;		
typedef struct _NT_PROC_THREAD_ATTRIBUTE_ENTRY {
	DWORD32 Attribute;    // PROC_THREAD_ATTRIBUTE_XXX，参见MSDN中UpdateProcThreadAttribute的说明
    SIZE_T Size;        // Value的大小
    ULONG_PTR Value;    // 保存4字节数据（比如一个Handle）或数据指针
    DWORD32 Unknown;      // 总是0，可能是用来返回数据给调用者
} PROC_THREAD_ATTRIBUTE_ENTRY, *PPROC_THREAD_ATTRIBUTE_ENTRY;
typedef struct _NT_PROC_THREAD_ATTRIBUTE_LIST {
    DWORD32 Length;       // 结构总大小
    PROC_THREAD_ATTRIBUTE_ENTRY Entry[1];
} NT_PROC_THREAD_ATTRIBUTE_LIST, *PNT_PROC_THREAD_ATTRIBUTE_LIST;
typedef struct _RTL_USER_PROCESS_PARAMETERS {
  CHAR           Reserved1[16];
  PVOID          Reserved2[10];
  UNICODE_STRING ImagePathName;
  UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;
typedef struct _SYSTEM_LOAD_AND_CALL_IMAGE 
{ 
 UNICODE_STRING ModuleName; 
} SYSTEM_LOAD_AND_CALL_IMAGE, *PSYSTEM_LOAD_AND_CALL_IMAGE; 
extern PKSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable;
typedef struct _NT_FUNCTION_
{
	PVOID ProxyAddress;
    PVOID* OrigAddress;
}NT_FUNCTION,*PNT_FUNCTION;
typedef struct _SYSTEM_HANDLE_INFORMATION
{
	ULONG ProcessID;
	UCHAR ObjectTypeNumber;
	UCHAR Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
}SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;
typedef struct _SYSTEM_HANDLE_INFORMATION_EX
{
	ULONG NumberOfHandles;
	SYSTEM_HANDLE_INFORMATION Information[1];
}SYSTEM_HANDLE_INFORMATION_EX, *PSYSTEM_HANDLE_INFORMATION_EX;
NTSTATUS ZwQuerySystemInformation(
	IN ULONG SystemInformationClass,
	IN PVOID SystemInformation,
	IN ULONG SystemInformationLength,
	OUT PULONG ReturnLength);
/*========================================================================================================*/
typedef NTSTATUS (*NTTERMINATEPROCESS)( HANDLE ProcessHandle, NTSTATUS ExitStatus);
typedef NTSTATUS (*NTCREATESECTION) (OUT PHANDLE SectionHandle, ACCESS_MASK DesiredAccess,  POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL, PLARGE_INTEGER MaximumSize OPTIONAL, DWORD32 SectionPageProtection, DWORD32 AllocationAttributes, HANDLE FileHandle OPTIONAL);
typedef NTSTATUS (*NTSETVALUEKEY)( HANDLE KeyHandle, PUNICODE_STRING ValueName, DWORD32 TitleIndex, DWORD32 type1, PVOID Data, DWORD32 DataSize);
typedef NTSTATUS (*NTLOADDRIVER)( PUNICODE_STRING DriverServiceName);
typedef NTSTATUS (*NTOPENPROCESS) ( PHANDLE ProcessHandle,  ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientId);
typedef NTSTATUS (*NTOPENTHREADTOKENEX)(  HANDLE ThreadHandle, ACCESS_MASK DesiredAccess,  BOOLEAN OpenAsSelf, DWORD32 HandleAttributes, PHANDLE TokenHandle);
typedef NTSTATUS (*NTCREATEPROCESSEX)(OUT PHANDLE ProcessHandle,  ACCESS_MASK DesiredAccess,  POBJECT_ATTRIBUTES ObjectAttributes, HANDLE InheritFromProcessHandle, HANDLE InheritHandles, HANDLE SectionHandle OPTIONAL, HANDLE DebugPort OPTIONAL, HANDLE ExceptionPort OPTIONAL, HANDLE Unknown);
typedef NTSTATUS (*NTCREATEUSERPROCESS)(OUT PHANDLE ProcessHandle,OUT PHANDLE ThreadHandle, ACCESS_MASK ProcessDesiredAccess, ACCESS_MASK ThreadDesiredAccess, POBJECT_ATTRIBUTES ProcessObjectAttributes OPTIONAL, POBJECT_ATTRIBUTES ThreadObjectAttributes OPTIONAL, DWORD32 CreateProcessFlags, DWORD32 CreateThreadFlags, PRTL_USER_PROCESS_PARAMETERS ProcessParameters, PVOID Parameter9, PNT_PROC_THREAD_ATTRIBUTE_LIST AttributeList);
typedef NTSTATUS (*NTOPENFILE)( PHANDLE FileHandle,  ACCESS_MASK DesiredAccess,  POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, ULONG ShareAccess,  ULONG OpenOptions);
typedef NTSTATUS (*NTDELETEFILE)( POBJECT_ATTRIBUTES ObjectAttributes);
typedef NTSTATUS (*NTCREATEFILE)( PHANDLE FileHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, PLARGE_INTEGER AllocationSize, ULONG FileAttributes, ULONG ShareAccess,  ULONG CreateDisposition, ULONG CreateOptions, PVOID EaBuffer, ULONG EaLength);
typedef NTSTATUS (*NTDELETEVALUEKEY)( HANDLE KeyHandle, PUNICODE_STRING ValueName);
typedef NTSTATUS (*NTSETSYSTEMINFORMATION)( ULONG ControlNumber, PVOID Information, ULONG SizeofInformation);
typedef NTSTATUS (*NTCREATEPROCESSEX)(PHANDLE ProcessHandle,ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes,HANDLE InheritFromProcessHandle,HANDLE InheritHandles,HANDLE SectionHandle,HANDLE DebugPort,HANDLE ExceptionPort,HANDLE Unknown);
//typedef NTSTATUS (*NTUSERPOSTMESSAGE)();
//typedef NTSTATUS (*NTUSERFINDWINDOWEX)();
//typedef NTSTATUS (*NTUSERGETFOREGROUNDWINDOW)();
typedef	NTSTATUS (*NTUSERMESSAGECALL)(PVOID Handle, INT Message, INT P1, INT P2);
typedef	NTSTATUS (*NTUSERCALLONEPARAM)(DWORD32 Param, DWORD32 Routine);
typedef	NTSTATUS(*NTUSERCALLNOPARAM)(DWORD32 Routine);
typedef NTSTATUS (*NTUSERFINDWINDOWEX)(PVOID hwndParent,PVOID hwndChild,PUNICODE_STRING pstrClassName,PUNICODE_STRING pstrWindowName,DWORD32 dwType);
typedef NTSTATUS (*NTUSERBUILDHWNDLIST)(PVOID hdesk,PVOID hwndNext,ULONG fEnumChildren,DWORD32 idThread,UINT32 cHwndMax,PVOID *phwndFirst,ULONG *pcHwndNeeded);
typedef UINT_PTR (*NTUSERQUERYWINDOW)(ULONG WindowHandle,ULONG TypeInformation);
typedef ULONG (*NTUSERGETFOREGROUNDWINDOW)();
typedef PVOID (*NTUSERWINDOWFROMPOINT)(LONG, LONG);
NTTERMINATEPROCESS		RealNtTerminateProcess;
NTCREATESECTION			RealNtCreateSection;
NTSETVALUEKEY			RealNtSetValueKey;
NTLOADDRIVER			RealNtLoadDriver;
NTOPENPROCESS			RealNtOpenProcess;
NTOPENTHREADTOKENEX		RealNtOpenThreadTokenEx;
NTCREATEUSERPROCESS		RealNtCreateUserProcess;
NTOPENFILE				RealNtOpenFile;
NTDELETEFILE			RealNtDeleteFile;
NTCREATEFILE			RealNtCreateFile;
NTDELETEVALUEKEY		RealNtDeleteValueKey;
NTSETSYSTEMINFORMATION	RealNtSetSystemInformation;
NTCREATEPROCESSEX       RealNtCreateProcessEx;
NTUSERMESSAGECALL		RealNtUserMessageCall;
NTUSERCALLONEPARAM      RealNtUserCallOneParam;
NTUSERCALLNOPARAM		RealNtUserCallNoParam;
/*DWORD32 NtTerminateProcessIndex;
DWORD32 NtCreateSectionIndex;
DWORD32 NtSetValueKeyIndex;
DWORD32 NtLoadDriverIndex;
DWORD32 NtOpenProcessIndex;
DWORD32 NtOpenThreadTokenExIndex;
DWORD32 NtCreateUserProcessIndex;
DWORD32 NtOpenFileIndex;
DWORD32 NtDeleteFileIndex;
DWORD32 NtCreateFileIndex;
DWORD32 NtDeleteValueKeyIndex;
DWORD32 NtSetSystemInformationIndex;*/
PNT_FUNCTION HookedAddressList;