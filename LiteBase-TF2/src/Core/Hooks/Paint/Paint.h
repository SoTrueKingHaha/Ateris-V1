#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace Paint
	{
		inline CHook Hook;
		using fn = void(__fastcall *)(void *, void *, int);
		void __fastcall Func(void *ecx, void *edx, int mode);
		void Init();
	}
}