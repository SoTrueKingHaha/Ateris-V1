#pragma once
#include "../../SDK/SDK.h"
#include "../../Features/Exploits/CritHack/CritHack.h"

namespace Hooks
{
	namespace IsAllowedToWithdrawFromCritBucket
	{
		inline CHook Hook;
		using fn = bool( __fastcall * )( void *, void *, float );
		bool __fastcall Func( void *ecx, void *edx, float flDamage );
		void Init( );
	}
}