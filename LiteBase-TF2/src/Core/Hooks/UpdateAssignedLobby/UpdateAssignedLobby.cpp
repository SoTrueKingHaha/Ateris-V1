#include "UpdateAssignedLobby.h"

char __fastcall Hooks::UpdateAssignedLobby::Func( void *ecx, void *edx )
{
	if ( !ecx || !edx )
	{
		return Hook.CallOriginal<fn>( )( ecx, edx );
	}

	char cReturn = Hook.CallOriginal<fn>( )( ecx, edx );
	if ( ecx )
	{
		*( ( bool * )ecx + 1440 ) = false;
	}
	return cReturn;
}

void Hooks::UpdateAssignedLobby::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC 28 53 8B D9 56 57 89 5D F0 8B 8B ? ? ? ? 85 C9 0F 84 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 85 C0 74 77" ) );
	Hook.Create( pSrc, Func );
}