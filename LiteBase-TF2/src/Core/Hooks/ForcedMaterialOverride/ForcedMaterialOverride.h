#pragma once
#include "../../SDK/SDK.h"
#include "../../Features/Visuals/Models/Models.h"
#include "../../Features/Visuals/Outlines/Outlines.h"

namespace Hooks {

	namespace ForcedMaterialOverride
	{
		inline CHook Func;
		using fn = void(__thiscall*)(IVModelRender*, IMaterial*, OverrideType_t);
		EXPORTED void __stdcall Hook(IMaterial* mat, OverrideType_t type);
		void Init();
	}
}