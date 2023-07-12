#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace LevelInitPreEntity
	{
		inline CHook Hook;
		using fn = void(__fastcall *)(void *, void *, char const *);
		void __fastcall Func(void *ecx, void *edx, char const *pMapName);
		void Init();
	}
}