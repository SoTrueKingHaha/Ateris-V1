#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace OnScreenSizeChanged
	{
		inline CHook Hook;
		using fn = void(__fastcall *)(void *, void *, int, int);
		void __fastcall Func(void *ecx, void *edx, int OldWidht, int OldHeight);
		void Init();
	}
}