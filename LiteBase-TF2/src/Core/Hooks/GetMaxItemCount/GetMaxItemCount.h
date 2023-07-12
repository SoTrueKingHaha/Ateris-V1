#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace GetMaxItemCount
	{
		inline CHook Hook;
		using fn = int( __fastcall * )( void *, void * );
		int __fastcall Func( void *ecx, void *edx );
		void Init( );
	}
}