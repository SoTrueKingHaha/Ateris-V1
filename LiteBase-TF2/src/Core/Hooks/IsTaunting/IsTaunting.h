#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace IsTaunting
	{
		inline CHook Hook;
		using fn = bool( __thiscall * )( );
		bool __stdcall Func( );
		void Init( );
	}
}