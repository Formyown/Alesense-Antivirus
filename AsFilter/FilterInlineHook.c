#include "FilterInlineHook.h"


BOOLEAN AsFilterSafeInlineHook(PVOID TargetAddress, PVOID ReadyOpCode, ULONG OpCodeLength)
{
    BOOLEAN result = FALSE;
    DPC_CONTEXT DpcContext;
    KAFFINITY OrigAffinity;
    UNICODE_STRING NameString;
    CCHAR CurrentProcessor;
    CCHAR Processor;
    PKDPC Dpc;
    ULONG i;
    KIRQL OrigIrql;
    OrigIrql = KeRaiseIrqlToDpcLevel();
    if (KeNumberProcessors > 1) {

        CurrentProcessor = (CCHAR)KeGetCurrentProcessorNumber();
        DpcContext.Dpcs = (PKDPC)ExAllocatePoolWithTag(NonPagedPool, KeNumberProcessors * sizeof(KDPC), 'SfHk');
        DpcContext.LockedProcessors = 1;
        DpcContext.ReleaseFlag = FALSE;
        for (Processor = 0; Processor < KeNumberProcessors; Processor++)
        {
            if (Processor == CurrentProcessor)  continue;
            Dpc = &DpcContext.Dpcs[Processor];
			KeInitializeDpc(Dpc,(PKDEFERRED_ROUTINE)SafeHookDpcRoutine, &DpcContext);
            KeSetTargetProcessorDpc(Dpc, Processor);
            KeInsertQueueDpc(Dpc, NULL, NULL);
        }

        for (i = 0; i < 0x800000; i++) {
            __asm   pause;
            if (DpcContext.LockedProcessors == (ULONG)KeNumberProcessors) break;
        }
        
        if (DpcContext.LockedProcessors != (ULONG)KeNumberProcessors) {
            KdPrint(("[AsFilter]Insert DPC Failed use unsafe way"));
            DpcContext.ReleaseFlag = TRUE;
            for (Processor = 0; Processor < KeNumberProcessors; Processor++) 
            {
                if (Processor != CurrentProcessor) {
                    KeRemoveQueueDpc(&DpcContext.Dpcs[Processor]);
                }
            }
			OpSafeInlineHook(TargetAddress, ReadyOpCode, OpCodeLength);
            result = TRUE;
        } else {
            KdPrint(("[AsFilter]starting safe inline hook"));
            OpSafeInlineHook(TargetAddress, ReadyOpCode, OpCodeLength);
            result = TRUE;
            DpcContext.ReleaseFlag = TRUE;  
        }
        do {
            __asm   pause;
        } while (DpcContext.LockedProcessors != 1);

        ExFreePoolWithTag(DpcContext.Dpcs, 'SfHk');

    } else {

        OpSafeInlineHook(TargetAddress, ReadyOpCode, OpCodeLength);
        result = TRUE;
    }
    KeLowerIrql(OrigIrql);
    return result;
}
void SafeHookDpcRoutine ( __in struct _KDPC *Dpc,__in_opt PDPC_CONTEXT DeferredContext,__in_opt PVOID SystemArgument1,__in_opt PVOID SystemArgument2)
{
    InterlockedIncrement(&DeferredContext->LockedProcessors);
    do {
        __asm   pause;
    } while (DeferredContext->ReleaseFlag == FALSE);
    InterlockedDecrement(&DeferredContext->LockedProcessors);
}
void OpSafeInlineHook(PVOID TargetAddress, PVOID ReadyOpCode, ULONG OpCodeLength)
{
       WriteProtectOff();
       RtlCopyMemory(TargetAddress, ReadyOpCode, OpCodeLength);
       WriteProtectOn();
}

void WriteProtectOff()
{
	_asm 
	{ 
		CLI;             
		MOV EAX, CR0;  
		AND EAX, NOT 10000H;
		MOV CR0, EAX;     
	} 
	
}
void WriteProtectOn()
{
	_asm  
	{ 
		MOV  EAX, CR0;        
		OR  EAX, 10000H;             
		MOV  CR0, EAX;               
		STI;                   
	} 
}