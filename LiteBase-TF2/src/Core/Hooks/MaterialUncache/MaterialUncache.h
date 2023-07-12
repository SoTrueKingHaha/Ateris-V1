#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace MaterialUncache
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( IMaterial *, void *, bool );
		void __fastcall Func( IMaterial *ecx, void *edx, bool bPreserveVars );
		void Init( );
	}
}