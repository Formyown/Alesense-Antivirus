#include "FilterFrame.h"
#include "FilterRoutine.h"
#include "FilterInlineHook.h"
#include "FilterCommunication.h"
PEPROCESS    crsEProc = NULL;
INT HOOKEDSIZE = 6;
const unsigned char HookCodeXP[5] = {0x8b,0x3f,0x8b,0x1c,0x87};
const unsigned char HookCodeNT[6] = {0x8a,0x0c,0x10,0x8b,0x14,0x87};
const unsigned char *CodeToReplace = HookCodeNT;
DWORD32 KiFastCallBaseAddress;
DWORD32 JumpAddress;
UCHAR HookedCode[6];
INT32 ReplaceOffset;
KDPC DPC_HookRecovery;
KTIMER TIMER_HookRecovery;
BOOLEAN LoadFrame()
{
	BOOLEAN Result;
	LoadProxyService();
	Result = SysEnterHookInline();
	if(Result == TRUE)
	{
		DbgPrint("Good!\n");
		InitlizeCommunication();
		StartHookRecovery();
	}
	return Result;
}
void UnLoadFrame()
{
	StopHookRecovery();
	UnSysEnterHookInline();
	ExFreePoolWithTag(HookedAddressList,'SDL');
}

void StartHookRecovery()
{
	LARGE_INTEGER Duetime = {0};
	KeInitializeTimerEx(&TIMER_HookRecovery,NotificationTimer);
	KeInitializeDpc(&DPC_HookRecovery,HookRecoveryRoutine,NULL);
	KeSetTimerEx(&TIMER_HookRecovery,Duetime, 100, &DPC_HookRecovery);  
	
}
void StopHookRecovery()
{
	if(!KeCancelTimer(&TIMER_HookRecovery))
		DbgPrint("Cancel Timer failed\n");
	else
		DbgPrint("Cancel Timer successfully\n");
}
void LoadProxyService()
{
	//NtPID = PsGetCurrentProcessId();
	HookedAddressList = (PNT_FUNCTION)ExAllocatePoolWithTag(NonPagedPool, 0x140c * sizeof(NT_FUNCTION), 'SDL');
	RtlZeroMemory(HookedAddressList, 0x140c * sizeof(NT_FUNCTION));
	//ShadowHookedAddressList = (PNT_FUNCTION)ExAllocatePoolWithTag(NonPagedPool, KeServiceDescriptorTable->win32k.NumberOfService*sizeof(NT_FUNCTION), 'SSL');
	//RtlZeroMemory(ShadowHookedAddressList, KeServiceDescriptorTable->win32k.NumberOfService*sizeof(NT_FUNCTION));
	HookedAddressList[SYSCALL_INDEX(ZwTerminateProcess)].ProxyAddress = HookNtTerminateProcess;
	//HookedAddressList[SYSCALL_INDEX(ZwCreateSection)].ProxyAddress = HookNtCreateSection;
	HookedAddressList[SYSCALL_INDEX(ZwSetValueKey)].ProxyAddress = HookNtSetValueKey;
	HookedAddressList[SYSCALL_INDEX(ZwLoadDriver)].ProxyAddress = HookNtLoadDriver;
	HookedAddressList[SYSCALL_INDEX(ZwOpenProcess)].ProxyAddress = HookNtOpenProcess;
	HookedAddressList[SYSCALL_INDEX(ZwDeleteValueKey)].ProxyAddress = HookNtDeleteValueKey;
	LoadProxyHardCodeService();
	RealNtTerminateProcess	= (NTTERMINATEPROCESS)SYSCALL_FUNCTION_ADDR(ZwTerminateProcess);
	RealNtCreateSection		= (NTCREATESECTION)(SYSCALL_FUNCTION_ADDR(ZwCreateSection));
	RealNtSetValueKey		= (NTSETVALUEKEY)(SYSCALL_FUNCTION_ADDR(ZwSetValueKey));
	RealNtLoadDriver		= (NTLOADDRIVER)(SYSCALL_FUNCTION_ADDR(ZwLoadDriver));
	RealNtOpenProcess		= (NTOPENPROCESS)(SYSCALL_FUNCTION_ADDR(ZwOpenProcess));
	RealNtOpenFile			= (NTOPENFILE)(SYSCALL_FUNCTION_ADDR(ZwOpenFile));
	RealNtDeleteFile		= (NTDELETEFILE)(SYSCALL_FUNCTION_ADDR(ZwDeleteFile));
	RealNtCreateFile		= (NTCREATEFILE)(SYSCALL_FUNCTION_ADDR(ZwCreateFile));
	RealNtDeleteValueKey	= (NTDELETEVALUEKEY)(SYSCALL_FUNCTION_ADDR(ZwDeleteValueKey));
}
void LoadProxyHardCodeService()
{
		RTL_OSVERSIONINFOW Version;
	RtlGetVersion(&Version); //获取系统信息
	switch (Version.dwMajorVersion)
	{
	case 5: 
		switch(Version.dwMinorVersion)
		{
		case 0://win2k
		//no support now
		break;
		case 1://winxp
		//no support now
			CodeToReplace = HookCodeXP;
			HOOKEDSIZE = 5;
			//================
			RealNtUserCallOneParam = 0;
			HookedAddressList[0x1143].ProxyAddress = HookNtUserCallOneParam;
			HookedAddressList[0x1143].OrigAddress = &RealNtUserCallOneParam;
			//=================
			HookedAddressList[0x30].ProxyAddress = HookNtCreateProcessEx;
			RealNtCreateProcessEx = (NTCREATEPROCESSEX)(SYSCALL_FUNCTION_ADDR_BY_INDEX(0x30));
			HookedAddressList[0x00f0].ProxyAddress = HookNtSetSystemInformation;
			RealNtSetSystemInformation    = (NTSETSYSTEMINFORMATION)(SYSCALL_FUNCTION_ADDR_BY_INDEX(0x00f0));
		break;
		default:
		break;
		}
		break;
	case 6:
		switch(Version.dwMinorVersion)
		{
		case 0:
		/*=======================//vista//=====================*/
		HookedAddressList[0x017f].ProxyAddress = HookNtCreateUserProcess;
		RealNtCreateUserProcess = (NTCREATEUSERPROCESS)(SYSCALL_FUNCTION_ADDR_BY_INDEX(0x017f));
		HookedAddressList[0x013d].ProxyAddress = HookNtSetSystemInformation;
		RealNtSetSystemInformation    = (NTSETSYSTEMINFORMATION)(SYSCALL_FUNCTION_ADDR_BY_INDEX(0x013d));
		HookedAddressList[0x114c].ProxyAddress = HookNtUserCallNoParam;
		HookedAddressList[0x114c].OrigAddress = &RealNtUserCallNoParam;
		//Vista SP1 and later
		if(SYSCALL_INDEX(ZwClose)==0x002f) 
		{
		HookedAddressList[0x0185].ProxyAddress = HookNtCreateUserProcess;
		RealNtCreateUserProcess = (NTCREATEUSERPROCESS)(SYSCALL_FUNCTION_ADDR_BY_INDEX(0x0185));
		HookedAddressList[0x0141].ProxyAddress = HookNtSetSystemInformation;//Vista SP0
		RealNtSetSystemInformation    = (NTSETSYSTEMINFORMATION)(SYSCALL_FUNCTION_ADDR_BY_INDEX(0x0141));
		
		}
		break;
		case 1:
		/*=======================//win7//=====================*/
		HookedAddressList[0x005d].ProxyAddress = HookNtCreateUserProcess;
		RealNtCreateUserProcess = (NTCREATEUSERPROCESS)(SYSCALL_FUNCTION_ADDR_BY_INDEX(0x005d));
		HookedAddressList[0x015e].ProxyAddress = HookNtSetSystemInformation;
		RealNtSetSystemInformation    = (NTSETSYSTEMINFORMATION)(SYSCALL_FUNCTION_ADDR_BY_INDEX(0x015e));
		HookedAddressList[0x114d].ProxyAddress = HookNtUserCallNoParam;
		HookedAddressList[0x114d].OrigAddress = &RealNtUserCallNoParam;
		break;
		case 2:
		/*=======================//win8//=====================*/
			if(SYSCALL_INDEX(ZwCreateSection)== 0x0150) //win8.0
		{
		HookedAddressList[0x0145].ProxyAddress = HookNtCreateUserProcess;
		RealNtCreateUserProcess = (NTCREATEUSERPROCESS)(SYSCALL_FUNCTION_ADDR_BY_INDEX(0x0145));
		HookedAddressList[0x0038].ProxyAddress = HookNtSetSystemInformation;
		RealNtSetSystemInformation	      = (NTSETSYSTEMINFORMATION)(SYSCALL_FUNCTION_ADDR_BY_INDEX(0x0038));
		//=========
		RealNtUserCallNoParam = 0;
		HookedAddressList[0x115e].ProxyAddress = HookNtUserCallNoParam;
		HookedAddressList[0x115e].OrigAddress = &RealNtUserCallNoParam;
		ShutdownRoutine = 17;
		//=======================
		DbgPrint("[AsFilter]Running on win8.0");
		}
		//===============================
		/*if(SYSCALL_INDEX(ZwCreateSection) == 0x014d)//win8 preview not support now
		{
		HookedAddressList[0x0142].ProxyAddress = HookNtCreateUserProcess;
		RealNtCreateUserProcess = (NTCREATEUSERPROCESS)(SYSCALL_FUNCTION_ADDR_BY_INDEX(0x0142));
		HookedAddressList[0x0038].ProxyAddress = HookNtSetSystemInformation;
		RealNtSetSystemInformation	      = (NTSETSYSTEMINFORMATION)(SYSCALL_FUNCTION_ADDR_BY_INDEX(0x0038));
		DbgPrint("[AsFilter]Running on win8 preview");
		ShutdownRoutine = 17;
		}*/
		//=================================
		break;
		case 3:
			if(SYSCALL_INDEX(ZwCreateSection) == 0x0154) //win8.1
			{
			HookedAddressList[0x0149].ProxyAddress = HookNtCreateUserProcess;
			RealNtCreateUserProcess = (NTCREATEUSERPROCESS)(SYSCALL_FUNCTION_ADDR_BY_INDEX(0x0149));
			HookedAddressList[0x0039].ProxyAddress = HookNtSetSystemInformation;
			RealNtSetSystemInformation	      = (NTSETSYSTEMINFORMATION)(SYSCALL_FUNCTION_ADDR_BY_INDEX(0x0039));
			DbgPrint("[AsFilter]Running on win8.1");
			HookedAddressList[0x1160].ProxyAddress = HookNtUserCallNoParam;
			HookedAddressList[0x1160].OrigAddress = &RealNtUserCallNoParam;
			ShutdownRoutine = 17;
			}
		break;
		default:
		DbgPrint("[AsFilter]UnknownMinorVersion=%i\n",Version.dwMinorVersion);
		break;
		}
		break;
	default:
		DbgPrint("[AsFilter]UnknownMajorVersion=%i\n",Version.dwMajorVersion);
		break;
	}

}
BOOLEAN SysEnterHookInline()
{
	BOOLEAN FindMov = FALSE;
	INT16 Index;
	INT32 TmpAddr;
		_asm
	{
		push ecx;
		push eax;
		mov ecx,176h;//代号176 CS_EIP
		rdmsr;
		mov KiFastCallBaseAddress,eax;
		pop eax;
		pop ecx;
	}
	//找到
	//819d22b6 8a0c10          mov     cl,byte ptr [eax+edx]
	//819d22b9 8b1487          mov     edx,dword ptr [edi+eax*4]
	for(ReplaceOffset = 0;ReplaceOffset < 1024;ReplaceOffset ++)
	{
		if(*((PUCHAR)(KiFastCallBaseAddress + ReplaceOffset)) == CodeToReplace[0])
		{
			FindMov = TRUE;
			for(Index = 0;Index < HOOKEDSIZE;Index ++)
			{
				if(*((PUCHAR)(KiFastCallBaseAddress + ReplaceOffset + Index)) != CodeToReplace[Index]) 
				{
					FindMov=FALSE;
					break;
				}
			}
			if(FindMov == TRUE) goto NXT;
		}
	}
NXT:
	if(ReplaceOffset >= 1024 - 5)
	{
		return FALSE;
	}
	JumpAddress = KiFastCallBaseAddress + ReplaceOffset + HOOKEDSIZE;
	if(HOOKEDSIZE == 5)
	TmpAddr = (INT32)&KiFastCallInlineXP - (KiFastCallBaseAddress + ReplaceOffset) - 5;
	else
	TmpAddr = (INT32)&KiFastCallInline - (KiFastCallBaseAddress + ReplaceOffset) - 5;

	HookedCode[0] = 0xE9;
	*((PINT32)(&HookedCode[1])) = TmpAddr;
	if(HOOKEDSIZE >= 6)HookedCode[5] = 0x90;
	return AsFilterSafeInlineHook((PVOID)(KiFastCallBaseAddress + ReplaceOffset),HookedCode,HOOKEDSIZE);
}
BOOLEAN UnSysEnterHookInline()
{
	LARGE_INTEGER DelayTime;
	IsOfflineMode = TRUE;
	DelayTime.QuadPart=- 10 * 1000 * 3000;
	KeDelayExecutionThread(KernelMode,0,&DelayTime);
	return AsFilterSafeInlineHook((PVOID)(KiFastCallBaseAddress + ReplaceOffset),(PVOID)CodeToReplace,HOOKEDSIZE);
}
void HookRecoveryRoutine(__in struct _KDPC *Dpc,__in_opt PVOID DeferredContext,__in_opt PVOID SystemArgument1,__in_opt PVOID SystemArgument2)
{
	if(!IsOfflineMode)
	{
		if(RtlCompareMemory((PVOID)(KiFastCallBaseAddress + ReplaceOffset),HookedCode,HOOKEDSIZE) != HOOKEDSIZE)
	{
		DbgPrint("[AsFilter]Hook was recoveried\n");
		AsFilterSafeInlineHook((PVOID)(KiFastCallBaseAddress + ReplaceOffset),HookedCode,HOOKEDSIZE);
	}
	}
}
__declspec(naked) void KiFastCallInline()
{
	_asm
	{
		mov  cl,byte ptr [eax+edx]
		mov  edx,dword ptr [edi+eax*4]
		pushfd
		pushad
		cmp IsOfflineMode,0x001
		je _Exit
		push edi
		push edx
		push eax
		call KiFastCallFilter
		mov [esp+14h],eax
_Exit:	
		popad 
		popfd
		mov eax,0
		push JumpAddress
		retn
	}
}
__declspec(naked) void KiFastCallInlineXP()
{
	_asm
	{
		mov edi,dword ptr [edi] 
		mov ebx,dword ptr [edi+eax*4] 
		pushfd
		pushad
		cmp IsOfflineMode,0x001
		je _Exit
		push edi
		push ebx
		push eax
		call KiFastCallFilter
		mov [esp+10h],eax
_Exit:	
		popad 
		popfd
		mov eax,0
		push JumpAddress
		retn
	}
}
DWORD32 KiFastCallFilter(DWORD32 ServiceId,UINT_PTR OriginalAddress,PUINT_PTR BaseAddr)
{
	if(BaseAddr == KeServiceDescriptorTable->ntoskrnl.ServiceTableBase && 
		ServiceId <= KeServiceDescriptorTable->ntoskrnl.NumberOfService)
	{
	if(HookedAddressList[ServiceId].ProxyAddress != NULL)
	{
		return (DWORD32)HookedAddressList[ServiceId].ProxyAddress;
	}
	}
else
	{
		
		ServiceId += 0x1000;
		if (ServiceId > KeServiceDescriptorTable->ntoskrnl.NumberOfService && ServiceId <= 0x140c)
		{
			
			
			if (HookedAddressList[ServiceId].ProxyAddress != NULL)
			{
				if (*HookedAddressList[ServiceId].OrigAddress == NULL)
					*(HookedAddressList[ServiceId].OrigAddress) = OriginalAddress;
				return (DWORD32)HookedAddressList[ServiceId].ProxyAddress;
			}
		}//Shadow SSDT
	}
	
		return OriginalAddress;
		
}
