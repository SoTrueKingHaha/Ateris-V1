#include "PostProcessing.h"

void __cdecl Hooks::PostProcessing::Func( int x, int y, int w, int h, bool bFlashlightIsOn, bool bPostVGui )
{
	return;
}

void Hooks::PostProcessing::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 81 EC ? ? ? ? 8B 0D ? ? ? ? 53 33 DB 89 5D 8C 89 5D 90 8B 01 89 5D A4 89 5D A8 56 57 85 C0 74 3C" ) );
	Hook.Create( pSrc, Func );
}