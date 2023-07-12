#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace CL_Move
	{
		inline CHook Hook;
		using fn = void( __cdecl * )( float, bool );
		void __cdecl Func( float flAccumulatedExtraSamples, bool bFinalTick );
		void Init( );
	}
}