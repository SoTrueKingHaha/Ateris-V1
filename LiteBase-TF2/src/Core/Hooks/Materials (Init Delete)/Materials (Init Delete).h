#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace Connect
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, const char*, int, INetChannel*, bool, int );
		void __fastcall Func( void *ecx, void *edx, const char *szName, int nUserID, INetChannel *pNetChannel, bool bFakePlayer, int clientChallenge );
		void Init( );
	}

	namespace Disconnect
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, const char * );
		void __fastcall Func( void *ecx, void *edx, const char *fmt );
		void Init( );
	}
}