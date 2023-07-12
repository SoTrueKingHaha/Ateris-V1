#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace DeleteIfUnreferenced
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( IMaterial *, void* );
		void __fastcall Func( IMaterial *ecx, void* edx );
		void Init( );
	}
}