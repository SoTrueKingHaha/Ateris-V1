#pragma once
#include "../../SDK/SDK.h"
#include "../../Features/Exploits/CritHack/CritHack.h"

namespace Hooks
{
	namespace AddToCritBucket
	{
		inline CHook Hook;
		using fn = void( __fastcall* )( void*, void*, float );
		void __fastcall Func( void *ecx, void *edx, float flAmount );
		void Init( );
	}
}