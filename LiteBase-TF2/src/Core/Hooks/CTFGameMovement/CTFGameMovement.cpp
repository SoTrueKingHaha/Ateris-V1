#include "CTFGameMovement.h"

void __cdecl Hooks::CTFGameMovement::Func( void *ecx, void *edx )
{
	if ( !I::CTFGameMovement )
	{
		I::CTFGameMovement = ecx;
	}

	Hook.CallOriginal<fn>( )( ecx, edx );
}

void Hooks::CTFGameMovement::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC 20 53 56 8B D9 57" ) );
	Hook.Create( pSrc, Func );
}