#pragma once
#include "../../SDK/SDK.h"
#include "../../Features/Vars.h"
#include "../../Features/Misc/Misc.h"

namespace Hooks
{
	namespace StartDynamicSound
	{
		inline CHook Hook;
		using fn = int (__cdecl* )(StartSoundParams_t& );
		int __cdecl Func( StartSoundParams_t& params);
		void Init( );
	}
}