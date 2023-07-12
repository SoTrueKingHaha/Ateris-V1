#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace CheckSimpleMaterial
	{
		inline CHook Hook;
		using fn = bool( __cdecl * )( IMaterial * );
		bool __cdecl Func( IMaterial * );
		void Init( );
	}
}