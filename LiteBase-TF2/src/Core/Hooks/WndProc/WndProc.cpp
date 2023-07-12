#include "WndProc.h"

LONG __stdcall Hooks::WndProc::Func(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return CallWindowProc(Original, hWnd, uMsg, wParam, lParam);
}

void Hooks::WndProc::Init()
{
	hwWindow = FindWindowW(0, L"Team Fortress 2");
	Original = (WNDPROC)SetWindowLongPtr(hwWindow, GWL_WNDPROC, (LONG_PTR)Func);
}