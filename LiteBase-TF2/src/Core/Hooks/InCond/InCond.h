#pragma once

#include "../../SDK/SDK.h"
#include <intrin.h>

namespace Hooks
{
	namespace InCond
	{
		inline CHook Hook;
		using fn = bool( __fastcall* )( void *, void *, ETFCond );
		bool __fastcall Func( void *ecx, void *edx, ETFCond cond );
		void Init( );
	}
}