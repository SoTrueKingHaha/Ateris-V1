#include "CanMoveDuringTaunt.h"

bool __fastcall Hooks::CanMoveDuringTaunt::Func( void *ecx, void *edx, float flInputSampleTime, CUserCmd *pCmd )
{
	*( DWORD * )( pCmd + 24 ) = 450.f;
	*( DWORD * )( pCmd + 32 ) = 450.f;
	return false;
}

void Hooks::CanMoveDuringTaunt::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC 0C A1 ? ? ? ? 56 57 8B F9 89 7D F4 A8 01 75 26 83 C8 01" ) );
	Hook.Create( pSrc, Func );
}

bool __fastcall Hooks::CanMoveDuringTaunt1::Func( void *ecx, void *edx )
{
	return true;
}

void Hooks::CanMoveDuringTaunt1::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.E8( L"client.dll", L"E8 ? ? ? ? 84 C0 75 13 C7 46 ? ? ? ? ? C7 46 ? ? ? ? ? E9 ? ? ? ?" ) );
	Hook.Create( pSrc, Func );
}

bool __fastcall Hooks::CanMoveDuringTaunt2::Func( void *ecx, void *edx )
{
	*( ( BYTE * ) edx + 7292 ) = true;
	return true;
}

void Hooks::CanMoveDuringTaunt2::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 51 56 8B F1 8B 0D ? ? ? ? 85 C9 74 3E" ) );
	Hook.Create( pSrc, Func );
}