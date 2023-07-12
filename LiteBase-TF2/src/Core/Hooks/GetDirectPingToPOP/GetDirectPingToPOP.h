#pragma once
#include "../../SDK/SDK.h"
#include "../../SDK/Include/Steam/ISteamNetworkingUtils.h"

namespace Hooks
{
	namespace GetDirectPingToPOP
	{
		inline CHook Hook;
		using fn = int( __fastcall * )( void*, void*, SteamNetworkingPOPID );
		int __fastcall Func( void *ecx, void *edx, SteamNetworkingPOPID popID );
		void Init( );
	}
}