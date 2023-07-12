#pragma once
#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace IsPlayerOnSteamFriendsList
	{
		inline CHook Hook;
		using fn = bool(__fastcall *)(void *, void *, CBasePlayer *);
		bool __fastcall Func(void *ecx, void *edx, CBasePlayer *pPlayer);
		void Init();
	}
}