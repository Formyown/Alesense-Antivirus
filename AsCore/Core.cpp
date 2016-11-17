#include "Core.h"
#include "Protect.h"
#include "Control.h"
CProtect* Protect;
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	IdentifyMod = new CIdentify();
	Protect = new CProtect();
	CControl* NtControl = new CControl();
	Protect->StartProtect();
	MessageBoxA(0,"good~","ÌבÊ¾",MB_ICONINFORMATION);
	Protect->PauseProtect();
	NtControl->~CControl();
	delete Protect;
return 0;
}
