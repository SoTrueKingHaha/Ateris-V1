#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace UpdateAssignedLobby
	{
		inline CHook Hook;
		using fn = char( __fastcall * )( void *, void * );
		char __fastcall Func( void *ecx, void *edx );
		void Init( );
	}
}