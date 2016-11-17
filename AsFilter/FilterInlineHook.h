#include "FilterEntry.h"

typedef struct _NULLDPC_CONTEXT {
 PKDPC Dpcs;
 ULONG LockedProcessors;
 BOOLEAN ReleaseFlag;
} DPC_CONTEXT, *PDPC_CONTEXT;
void WriteProtectOff();
void WriteProtectOn();
BOOLEAN AsFilterSafeInlineHook(PVOID TargetAddress, PVOID ReadyOpCode, ULONG OpCodeLength);
void SafeHookDpcRoutine ( __in struct _KDPC *Dpc,__in_opt PDPC_CONTEXT DeferredContext,__in_opt PVOID SystemArgument1,__in_opt PVOID SystemArgument2);
void OpSafeInlineHook(PVOID TargetAddress, PVOID ReadyOpCode, ULONG OpCodeLength);