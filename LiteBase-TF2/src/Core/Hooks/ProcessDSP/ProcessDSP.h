#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace ProcessDSP
	{
		inline CHook Hook;
		using fn = void( __cdecl * )( unsigned int, int *, int *, int *, int );
		void __cdecl Func( unsigned int idsp, int *pbfront, int *pbrear, int *pbcenter, int sampleCount );
		void Init( );
	}
}