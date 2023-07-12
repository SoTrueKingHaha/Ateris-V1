#include "GetClientInterpAmount.h"

float __fastcall Hooks::EngineClientInterpAmount::Func( void *ecx, void *edx )
{
	const float retValue = Hook.CallOriginal<fn>( )( ecx, edx );
	g_GlobalInfo.lerpTime = retValue;
	return retValue;
}

void Hooks::EngineClientInterpAmount::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"engine.dll", L"55 8B EC 8B 0D ? ? ? ? 83 EC 10 85 C9 75 26 8B 0D ? ? ? ? 68 ? ? ? ? 8B 01 FF 50 34" ) );
	Hook.Create( pSrc, Func );
}

float __cdecl Hooks::GetClientInterpAmount::Func( )
{
	return Vars::Misc::DisableInterpolation.var || g_GlobalInfo.m_bRecharging ? 0.f : Hook.CallOriginal<fn>( )( );
}

void Hooks::GetClientInterpAmount::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC A1 ? ? ? ? 83 EC 08 56 A8 01 75 22 8B 0D ? ? ? ? 83 C8 01 A3 ? ? ? ? 68 ? ? ? ? 8B 01 FF 50 34 8B F0 89 35 ? ? ? ? EB 06 8B 35 ? ? ? ? 85 F6 74 68 8B 06 8B CE 8B 40 3C FF D0 8B 0D" ) );
	Hook.Create( pSrc, Func );
}