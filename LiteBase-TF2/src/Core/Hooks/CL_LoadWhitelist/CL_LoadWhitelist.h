#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace CL_LoadWhitelist
	{
		inline CHook Hook;
		using fn = void *( __cdecl * )( void *, const char * );
		void *__cdecl Func( void *table, const char *name );
		void Init( );
	}
}