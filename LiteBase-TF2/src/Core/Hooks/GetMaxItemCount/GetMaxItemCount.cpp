#include "GetMaxItemCount.h"

int __fastcall Hooks::GetMaxItemCount::Func( void *ecx, void *edx )
{
	return 3000;
}

void Hooks::GetMaxItemCount::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"8B 49 68 56" ) );
	Hook.Create( pSrc, Func );
}