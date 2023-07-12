#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace DataTableWarning
	{
		inline CHook Hook;
		using fn = void( __cdecl * )( const char * );
		void __cdecl Func( const char *pInMessage );
		void Init( );
	}
}