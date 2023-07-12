#include "OnScreenSizeChanged.h"

void __fastcall Hooks::OnScreenSizeChanged::Func(void *ecx, void *edx, int OldWidht, int OldHeight)
{
	Hook.CallOriginal<fn>()(ecx, edx, OldWidht, OldHeight);
	g_Draw.ReloadFonts();
	g_Draw.UpdateScreenSize();
}

void Hooks::OnScreenSizeChanged::Init()
{
	fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(I::Surface, 111));
	Hook.Create(FN, Func);
}