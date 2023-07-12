#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace CanMoveDuringTaunt
	{
		inline CHook Hook;
		using fn = bool( __fastcall * )( void*, void*, float, CUserCmd* );
		bool __fastcall Func( void *ecx, void *edx, float flInputSampleTime, CUserCmd* pCmd );
		void Init( );
	}

	namespace CanMoveDuringTaunt1
	{
		inline CHook Hook;
		using fn = bool( __fastcall * )( void *, void * );
		bool __fastcall Func( void *ecx, void *edx );
		void Init( );
	}

	namespace CanMoveDuringTaunt2
	{
		inline CHook Hook;
		using fn = bool( __fastcall * )( void *, void * );
		bool __fastcall Func( void *ecx, void *edx );
		void Init( );
	}
}