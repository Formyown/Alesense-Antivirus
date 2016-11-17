#include "Control.h"


CControl::CControl()
{
	InitializeNtControl();
	AsCreateNamedPipe(L"AsCoreControl", OnCoreControl);
}


CControl::~CControl()
{
	TerminateNtControl();
}


void OnCoreControl(PVOID Data)
{
	PNTCONTROLMSG Msg = (PNTCONTROLMSG)Data;
	switch (Msg->ControlCode)
	{
	case ForceDelete:
		ForceDeleteFile(Msg->ControlMsg);
		break;
	default:
		break;
	}
}