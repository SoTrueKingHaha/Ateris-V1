#include "LockCursor.h"
#include "../../Features/Visuals/Render/GUI/Menu/GUI.h"

void __stdcall Hooks::LockCursor::Func()
{
	g_Menu.m_bOpen ? I::Surface->UnlockCursor() : Hook.CallOriginal<fn>()(I::Surface);
}

void Hooks::LockCursor::Init()
{
	fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(I::Surface, 62));
	Hook.Create(FN, Func);
}