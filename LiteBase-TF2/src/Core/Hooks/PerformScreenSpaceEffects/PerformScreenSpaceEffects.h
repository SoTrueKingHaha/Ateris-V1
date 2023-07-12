#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace PerformScreenSpaceEffects
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void*, void*, int, int, int, int );
		void __fastcall Func( void *ecx, void *edx, int x, int y, int w, int h );
		void Init( );
	}
}