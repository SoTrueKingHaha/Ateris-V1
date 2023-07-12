#include "LevelShutdown.h"
#include "../../Features/Visuals/Outlines/Outlines.h"

void __fastcall Hooks::LevelShutdown::Func(void *ecx, void *edx)
{
	Hook.CallOriginal<fn>()(ecx, edx);
	g_EntityCache.Clear();
	//Models.DeleteMaterials( );
	//G::mapFriends.clear();
}

void Hooks::LevelShutdown::Init()
{
	fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(I::BaseClientDLL, 7));
	Hook.Create(FN, Func);
}