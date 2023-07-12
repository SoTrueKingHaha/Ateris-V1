#pragma once
#include "../../SDK/SDK.h"
#include "../../Features/Visuals/Models/Models.h"
#include "../../Features/Visuals/Outlines/Outlines.h"

namespace Hooks
{
	namespace DoPostScreenSpaceEffects
	{
		inline CHook Func;
		using fn = bool(__thiscall*)(CClientModeShared*, const CViewSetup*);
		bool __stdcall Hook(const CViewSetup* pSetup);
		void Init();
	}
}