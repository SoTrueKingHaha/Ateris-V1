#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace RenderViewSetColorModulation
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, float const * );
		void __fastcall Func( void *ecx, void *edx, float const *blend );
		void Init( );
	}

	namespace RenderViewSetBlend
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, float );
		void __fastcall Func( void *ecx, void *edx, float flAlpha );
		void Init( );
	}
}