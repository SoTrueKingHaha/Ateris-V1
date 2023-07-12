#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace CL_SendMove
	{
		inline CHook Hook;
		using fn = void( __cdecl * )( void *, void * );
		void __cdecl Func( void *ecx, void *edx );
		void Init( );
	}
}