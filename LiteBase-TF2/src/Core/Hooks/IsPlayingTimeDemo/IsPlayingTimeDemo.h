#pragma once
#include "../../SDK/SDK.h"
#include "intrin.h"

namespace Hooks
{
	namespace IsPlayingTimeDemo
	{
		inline CHook Hook;
		using fn = bool( __fastcall * )( void *, void * );
		bool __fastcall Func( void *ecx, void *edx );
		void Init( );
	}
}