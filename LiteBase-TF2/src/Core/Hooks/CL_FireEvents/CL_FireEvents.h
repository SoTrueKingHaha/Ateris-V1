#pragma once
#include "../../SDK/SDK.h"
#include <intrin.h>

namespace Hooks
{
	namespace CL_FireEvents
	{
		inline CHook Hook;
		using fn = float( __fastcall * )( void *, void * );
		float __fastcall Func( void *ecx, void *edx );
		void Init( );
	}
}