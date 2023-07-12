#include "CritBucketAdd.h"

void __fastcall Hooks::AddToCritBucket::Func( void *ecx, void *edx, float flAmount )
{
	if ( g_Crits.ProtectData ) {
		return;
	}

	return Hook.CallOriginal<fn>( )( ecx, edx, flAmount );
}

void Hooks::AddToCritBucket::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC A1 ? ? ? ? F3 0F 10 81 ? ? ? ? F3 0F 10 48 ? 0F 2F C8 76 1D F3 0F 58 45 ? 0F 2F C8 F3 0F 11 81 ? ? ? ? 77 03 0F 28 C1 F3 0F 11 81 ? ? ? ? 5D C2 04 00" ) );
	Hook.Create( pSrc, Func );
}