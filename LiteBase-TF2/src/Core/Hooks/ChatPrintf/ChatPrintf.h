#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace ChatPrintf
	{
		inline CHook Hook;
		using fn = void( __cdecl * )( void*, int, int, const char*, ... );
		void __cdecl Func( void *ecx, int iPlayerIndex, int iFilter, const char *format, ... );
		void Init( );
	}
}