#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace LevelInitPostEntity
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void * );
		void __fastcall Func( void *ecx, void *edx );
		void Init();
	}
}