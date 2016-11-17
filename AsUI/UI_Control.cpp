#include "UI_Control.h"

void ForceDeleteFile(LPWSTR FilePath)
{
	NTCONTROLMSG Msg = { 0 };
	lstrcpy(Msg.ControlMsg, FilePath);
	Msg.ControlCode = ForceDelete;
	SendToCoreControl(&Msg);
}