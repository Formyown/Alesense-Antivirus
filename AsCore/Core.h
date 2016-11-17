#pragma once
#include <Windows.h>
#include "Identify.h"
void OnUIScanMessage(PVOID Data);
DWORD WINAPI FileScanThread(PVOID arg);