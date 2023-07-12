#include "ProcessDSP.h"

void __cdecl Hooks::ProcessDSP::Func( unsigned int idsp, int *pbfront, int *pbrear, int *pbcenter, int sampleCount )
{
	return;
}

void Hooks::ProcessDSP::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"engine.dll", L"55 8B EC 51 56 8B 75 08 83 FE 1F 0F 87 ? ? ? ? 8B 0D ? ? ? ? 8B 01 8B 40 2C FF D0 84 C0 0F 84 ? ? ? ? A1 ? ? ? ? 6B F6 70 81 C6 ? ? ? ? 83 78 30 00 0F 85 ? ? ? ? 83" ) );
	Hook.Create( pSrc, Func );
}