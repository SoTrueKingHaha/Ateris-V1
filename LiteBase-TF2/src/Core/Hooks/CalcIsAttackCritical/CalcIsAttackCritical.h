#pragma once
#include "../../SDK/SDK.h"
#include "../../Features/Exploits/CritHack/CritHack.h"

namespace Hooks
{
	namespace CalcIsAttackCritical
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void * );
		void __fastcall Func( void *ecx, void *edx );
		void Init( );
	}
}