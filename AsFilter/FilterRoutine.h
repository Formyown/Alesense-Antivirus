#include "FilterEntry.h"
#include "FilterFrame.h"
//===========================//
#define MAX_PROCESS 32
extern DWORD32 ShutdownRoutine;//win7
//===========================//
BOOLEAN CompareWChar(LPWSTR string1,LPWSTR string2);
BOOLEAN FindWChar(LPWSTR string1,LPWSTR string2);
#define AutoRunSplit L"microsoft\\windows\\currentversion\\run"
#define IEMainPageSplit L"microsoft\\internet explorer\\main"
#define IFEOSplit L"currentversion\\image file execution options"
//===========================//
BOOLEAN InsertProtectedProcess(DWORD32 TargetPID);
BOOLEAN RemoveProtectedProcess(DWORD32 TargetPID);
BOOLEAN IsSafeTransferation(DWORD32 TargetPID,DWORD32 TransferPID);
BOOLEAN IsSafeProcess(DWORD32 TransferPID);
//===========================//
NTSTATUS HookNtTerminateProcess(__in_opt HANDLE ProcessHandle, NTSTATUS ExitStatus);
NTSTATUS HookNtCreateSection ( PHANDLE SectionHandle, ACCESS_MASK DesiredAccess,  POBJECT_ATTRIBUTES ObjectAttributes, PLARGE_INTEGER MaximumSize, DWORD32 SectionPageProtection, DWORD32 AllocationAttributes, HANDLE FileHandle);
NTSTATUS HookNtSetValueKey( HANDLE KeyHandle, PUNICODE_STRING ValueName, DWORD32 TitleIndex, DWORD32 type1, PVOID Data, DWORD32 DataSize);
NTSTATUS HookNtDeleteValueKey( HANDLE KeyHandle, PUNICODE_STRING ValueName);
NTSTATUS HookNtLoadDriver( PUNICODE_STRING DriverServiceName);
NTSTATUS HookNtOpenProcess ( PHANDLE ProcessHandle,  ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientId);
NTSTATUS HookNtOpenThreadTokenEx(  HANDLE ThreadHandle, ACCESS_MASK DesiredAccess,  BOOLEAN OpenAsSelf, DWORD32 HandleAttributes, PHANDLE TokenHandle);
NTSTATUS HookNtCreateUserProcess(OUT PHANDLE ProcessHandle,OUT PHANDLE ThreadHandle, ACCESS_MASK ProcessDesiredAccess, ACCESS_MASK ThreadDesiredAccess, POBJECT_ATTRIBUTES ProcessObjectAttributes OPTIONAL, POBJECT_ATTRIBUTES ThreadObjectAttributes OPTIONAL, DWORD32 CreateProcessFlags, DWORD32 CreateThreadFlags, PRTL_USER_PROCESS_PARAMETERS ProcessParameters, PVOID Parameter9, PNT_PROC_THREAD_ATTRIBUTE_LIST AttributeList);
NTSTATUS HookNtOpenFile( PHANDLE FileHandle,  ACCESS_MASK DesiredAccess,  POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, ULONG ShareAccess,  ULONG OpenOptions);
NTSTATUS HookNtDeleteFile( POBJECT_ATTRIBUTES ObjectAttributes);
NTSTATUS HookNtCreateFile( PHANDLE FileHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, PLARGE_INTEGER AllocationSize, ULONG FileAttributes, ULONG ShareAccess,  ULONG CreateDisposition, ULONG CreateOptions, PVOID EaBuffer, ULONG EaLength);
NTSTATUS HookNtSetSystemInformation( ULONG ControlNumber,  PVOID Information,  ULONG SizeofInformation);
NTSTATUS HookNtCreateProcessEx(PHANDLE ProcessHandle,ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes,HANDLE InheritFromProcessHandle,HANDLE InheritHandles,HANDLE SectionHandle,HANDLE DebugPort,HANDLE ExceptionPort,HANDLE Unknown);
NTSTATUS HookNtUserMessageCall(PVOID Handle, INT Message, INT P1, INT P2);
NTSTATUS HookNtUserCallOneParam(DWORD32 Param, DWORD32 Routine);
NTSTATUS HookNtUserCallNoParam(DWORD32 Routine);