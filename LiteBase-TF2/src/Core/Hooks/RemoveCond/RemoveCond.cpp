#include "RemoveCond.h"

void __fastcall Hooks::RemoveCond::Func( void *ecx, void *edx, ETFCond nCond, bool bIgnoreDuration )
{
	if ( nCond == TF_COND_STEALTHED )
	{
		return;
	}
	else
	{
		return Hook.CallOriginal<fn>( )( ecx, edx, nCond, bIgnoreDuration );
	}
}

void Hooks::RemoveCond::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 53 8B 5D 08 57 53 8B F9 E8 ? ? ? ? 84 C0 0F 84" ) );
	Hook.Create( pSrc, Func );
}