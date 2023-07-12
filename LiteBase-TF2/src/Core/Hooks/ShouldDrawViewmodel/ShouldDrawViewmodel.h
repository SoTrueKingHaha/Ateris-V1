#pragma once
#include "../../SDK/SDK.h"
#include "../../Features/Misc/Misc.h"

namespace Hooks
{
	namespace ShouldDrawViewmodel
	{
		inline CHook Hook;
		using fn = bool( __thiscall * )( CClientModeShared * );
		EXPORTED bool __stdcall Func( );
		void Init( );
	}
}