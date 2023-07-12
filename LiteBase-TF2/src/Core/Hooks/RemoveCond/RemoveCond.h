#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace RemoveCond
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, ETFCond, bool );
		void __fastcall Func( void *ecx, void *edx, ETFCond cond, bool bIgnoreDuration );
		void Init( );
	}
}