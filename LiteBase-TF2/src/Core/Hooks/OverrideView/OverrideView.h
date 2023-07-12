#pragma once
#include "../../SDK/SDK.h"
#include "../../Features/Misc/Misc.h"

namespace Hooks
{
	namespace OverrideView
	{
		inline CHook Hook;
		using fn = void(__thiscall*)(CClientModeShared*, CViewSetup*);
		void __stdcall Func(CViewSetup* pView);
		void Init();
	}
}