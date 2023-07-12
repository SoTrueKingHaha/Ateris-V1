#include "S_StartDynamicSound.h"

int __cdecl Hooks::StartDynamicSound::Func( StartSoundParams_t& params )
{
	g_Misc.ManualNetwork( params );
	return Hook.CallOriginal<fn>( )( params );
}

void Hooks::StartDynamicSound::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"engine.dll", L"55 8B EC 8B 0D ? ? ? ? 81 EC ? ? ? ? 85 C9" ) );
	Hook.Create( pSrc, Func );
}