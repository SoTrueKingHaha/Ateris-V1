#pragma once
#include "../../SDK/SDK.h"
#include "../../Features/Exploits/Backtrack/Backtrack.h"

namespace Hooks
{
	namespace LevelInit
	{
		inline CHook Hook;
		using fn = void( __fastcall* )( void *, void * );
		void __fastcall Func( void *ecx, void *edx );
		void Init( );
	}
}