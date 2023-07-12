#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace FrameAdvance
	{
		inline CHook Hook;
		using fn = float( __fastcall * )( void *, void *, float );
		float __fastcall Func( void *ecx, void *edx, float flInterval );
		void Init( );
	}
}