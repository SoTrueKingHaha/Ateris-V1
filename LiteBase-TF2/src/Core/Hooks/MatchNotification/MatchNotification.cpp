#include "MatchNotification.h"

void __fastcall Hooks::MatchNotification::Func( void *ecx, void *edx )
{
	if ( !ecx || !edx )
	{
		return;
	}

	*reinterpret_cast< double * >( reinterpret_cast< DWORD >( ecx ) + 424 ) = 0.0;
	Hook.CallOriginal<fn>( )( ecx, edx );
}

void Hooks::MatchNotification::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC 0C 56 8B F1 E8 ? ? ? ? 8B 86 ? ? ? ? 85 C0 74 09 83 F8 02 0F 85" ) );
	Hook.Create( pSrc, Func );
}