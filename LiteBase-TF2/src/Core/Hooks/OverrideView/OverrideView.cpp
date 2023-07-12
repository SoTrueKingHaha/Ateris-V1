#include "OverrideView.h"

void __stdcall Hooks::OverrideView::Func(CViewSetup* pView)
{
	Hook.CallOriginal<fn>()(I::ClientModeShared, pView);
	g_Misc.FOV(pView);
	g_Misc.ThirdPerson(pView);
}

void Hooks::OverrideView::Init()
{
	fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(I::ClientModeShared, 16));
	Hook.Create(FN, Func);
}