#pragma once
#include "../../SDK/SDK.h"
#include "../../Features/Vars.h"

namespace Hooks
{
	namespace ViewOffset
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, CTFPlayer *, Vec3 &, Vec3 & );
		void __fastcall Func( void *ecx, void *edx, CTFPlayer *ownerEntity, Vec3 &eyePosition, Vec3 &eyeAngles );
		void Init( );
	}
}