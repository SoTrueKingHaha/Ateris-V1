#include "CanRequestToJoinPlayer.h"

bool __fastcall Hooks::CanRequestToJoinPlayer::Func( CTFPartyClient *ecx, void *edx, int64 targetPlayer )
{
	return true;
}

void Hooks::CanRequestToJoinPlayer::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"engine.dll", L"55 8B EC 83 EC 08 8B 55 08 56 8B 75 0C 57 8B F9 89 55 F8 89 75 FC 8B 4F 18 85 C9 74 1E" ) );
	Hook.Create( pSrc, Func );
}