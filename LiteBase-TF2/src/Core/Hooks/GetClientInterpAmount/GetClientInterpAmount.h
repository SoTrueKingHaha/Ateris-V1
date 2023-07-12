#pragma once

#include "../../SDK/SDK.h"
namespace Hooks
{
	namespace EngineClientInterpAmount
	{
		inline CHook Hook;
		using fn = float( __fastcall * )( void *, void * );
		float __fastcall Func( void *ecx, void *edx );
		void Init( );
	}

	namespace GetClientInterpAmount
	{
		inline CHook Hook;
		using fn = float( __cdecl * )( );
		float __cdecl Func( );
		void Init( );
	}
}