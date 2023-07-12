#include "CritBucketWithdraw.h"

bool __fastcall Hooks::IsAllowedToWithdrawFromCritBucket::Func( void *ecx, void *edx, float flDamage )
{
	g_Crits.stats.flDamage = flDamage;
	if ( g_Crits.ProtectData ) {
		return true;
	}

	return Hook.CallOriginal<fn>( )( ecx, edx, flDamage );
}

void Hooks::IsAllowedToWithdrawFromCritBucket::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 56 8B F1 0F B7 86 ? ? ? ? FF 86 ? ? ? ? 50 E8 ? ? ? ? 83 C4 04 80 B8 ? ? ? ? ? 74 0A F3 0F 10 15" ) );
	Hook.Create( pSrc, Func );
}