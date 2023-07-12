#include "IsPlayerOnSteamFriendsList.h"

bool __fastcall Hooks::IsPlayerOnSteamFriendsList::Func(void *ecx, void *edx, CBasePlayer *pPlayer)
{
	bool bOriginal = Hook.CallOriginal<fn>()(ecx, edx, pPlayer);

	/*if ( pPlayer && bOriginal )
		G::mapFriends[pPlayer->As<CTFPlayer>()] = true;*/

	return bOriginal;
}

void Hooks::IsPlayerOnSteamFriendsList::Init()
{
	fn FN = reinterpret_cast<fn>(g_Pattern.Find(
		L"client.dll",
		L"55 8B EC 81 EC ? ? ? ? E8 ? ? ? ? 85 C0 74 7E 8B 4D 08 85 C9 74 77 A1 ? ? ? ? 83 78 08 00 74 6C 83 78 0C 00 74 66 A1 ? ? ? ? 83 C1 08 56 8B 30 8D 85 ? ? ? ? 50 8B 01"));
	Hook.Create(FN, Func);
}