#pragma once
#include "../../SDK/SDK.h"
#include "../../SDK/Include/TF2/CTFPartyClient.h"
#include "../../SDK/Include/Steam/SteamTypes.h"

namespace Hooks
{
	namespace CanRequestToJoinPlayer
	{
		inline CHook Hook;
		using fn = bool( __fastcall * ) ( CTFPartyClient *, void *, int64 );
		bool __fastcall Func( CTFPartyClient *ecx, void *edx, int64 targetPlayer );
		void Init( );
	}
}