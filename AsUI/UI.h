#include <windows.h>
void SetTrayText(LPWSTR Text);
void RemoveTrayIcon();
void AddTrayIcon(HWND hWnd, HINSTANCE hInst);
extern HWND g_hWnd;
typedef LRESULT(CALLBACK*WNDPROC)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc( //WndProc名称可自由定义
	HWND hwnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);