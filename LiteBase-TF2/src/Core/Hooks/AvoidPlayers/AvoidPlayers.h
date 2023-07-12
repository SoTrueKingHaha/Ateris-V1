#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace AvoidPlayers
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, CUserCmd * );
		void __fastcall Func( void * ecx, void * edx, CUserCmd * pCmd );
		void Init( );
	}
}