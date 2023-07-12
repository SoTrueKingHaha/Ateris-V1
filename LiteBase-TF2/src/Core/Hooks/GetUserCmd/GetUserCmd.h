#pragma once
#include "../../SDK/SDK.h"
#include "../../SDK/Include/TF2/input.h"
#include "../../Features/Exploits/CritHack/CritHack.h"

namespace Hooks
{
	namespace GetUserCmd
	{
		inline CHook Hook;
		using fn = CUserCmd * ( __thiscall * )( IInput *, int );
		CUserCmd *__stdcall Func( int sequence_number );
		void Init( );
	}
}