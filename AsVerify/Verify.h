#include <Windows.h>
#include <atlstr.h>
#include <atlconv.h>
#include <WinTrust.H>
#include <Mscat.h>
#include <windef.h>
#pragma comment (lib,"Wintrust.lib")
#define WINTRUST_ACTION_GENERIC_VERIFY_V2 {0xaac56b,0xcd44,0x11d0,{ 0x8c,0xc2,0x00,0xc0,0x4f,0xc2,0x95,0xee}} 
extern "C" __declspec(dllexport)  bool VerifyFileDigitalSignature(LPWSTR FileName);
extern "C" __declspec(dllexport)  bool VerifyFileDigitalSignatureByHandle(PWCHAR FileName,HANDLE FileHandle);