#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace GetOuterAbsVelocity
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, Vec3& );
		void __fastcall Func( void *ecx, void *edx, Vec3 &vec );
		void Init( );
	}
}