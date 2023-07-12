#pragma once

#include "../../SDK/SDK.h"
#include <intrin.h>

namespace Hooks
{
	namespace KeyValues
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, const char *, int );
		void __fastcall Func( void *ecx, void *edx, const char *keyName, int value );
		void Init( );
	}

	namespace IsPlayerDominated
	{
		inline CHook Hook;
		using fn = bool( __fastcall * )( void *, void *, int );
		bool __fastcall Func( void *ecx, void *edx, int index );
		void Init( );
	}
}