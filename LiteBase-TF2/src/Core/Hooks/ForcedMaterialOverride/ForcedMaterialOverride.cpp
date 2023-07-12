#include "ForcedMaterialOverride.h"

void __stdcall Hooks::ForcedMaterialOverride::Hook(IMaterial* mat, OverrideType_t type)
{
	if ((Outlines.m_bRendering && !Outlines.IsGlowMaterial(mat)) || (Models.m_bRendering && !Models.IsChamsMaterial(mat))) { return; }

	Func.CallOriginal<fn>()(I::ModelRender, mat, type);
}

void Hooks::ForcedMaterialOverride::Init()
{
	fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(I::ModelRender, 1));
	Func.Create(FN, Hook);
}