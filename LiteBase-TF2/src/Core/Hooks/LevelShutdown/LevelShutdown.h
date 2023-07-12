#pragma once
#include "../../SDK/SDK.h"
#include "../../SDK/Include/EntityCache/EntityCache.h"

namespace Hooks
{
	namespace LevelShutdown
	{
		inline CHook Hook;
		using fn = void(__fastcall *)(void *, void *);
		void __fastcall Func(void *ecx, void *edx);
		void Init();
	}
}