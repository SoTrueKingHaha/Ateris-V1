#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace PostProcessing
	{
		inline CHook Hook;
		using fn = void( __cdecl * )( int, int, int, int, bool, bool );
		void __cdecl Func( int x, int y, int w, int h, bool bFlashlightIsOn, bool bPostVGui );
		void Init( );
	}
}