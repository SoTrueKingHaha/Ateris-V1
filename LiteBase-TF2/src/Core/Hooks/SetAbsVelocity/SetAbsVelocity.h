#pragma once
#include "../../SDK/SDK.h"
#include "intrin.h"

namespace Hooks
{
	namespace SetAbsVelocity
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, const Vec3 & );
		void __fastcall Func( void *ecx, void *edx, const Vec3 &vecAbsVelocity );
		void Init( );
	}
}