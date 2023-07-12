#include "AvoidPlayers.h"

void __fastcall Hooks::AvoidPlayers::Func( void *ecx, void *edx, CUserCmd *pCmd )
{
	if ( !Vars::Misc::NoPush.var )
	{
		return Hook.CallOriginal<fn>( )( ecx, edx, pCmd );
	}
}

void Hooks::AvoidPlayers::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 57 8B F9 89 7D D4 83 78 30 00" ) );
	Hook.Create( pSrc, Func );
}