#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace DrawStaticProps
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, IClientRenderable **, int, bool, bool );
		void __fastcall Func( void *ecx, void *edx, IClientRenderable **pProps, int count, bool bShadowDepth, bool drawVCollideWireframe );
		void Init( );
	}

	namespace SetColorModulation
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, float const * );
		void __fastcall Func( void *ecx, void *edx, float const *pColor );
		void Init( );
	}

	namespace SetAlphaModulation
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, float );
		void __fastcall Func( void *ecx, void *edx, float flAlpha );
		void Init( );
	}
}